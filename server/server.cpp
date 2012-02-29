/*
 *  Copyright (C) <2008>  <chenkun cs.kunchen@gmail.com>

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "connectmysql.h"
#include "../client/error.h"
#include "util.h"
#include "config.h"
#include "log.h"
#include "taskProducer.h"
#include "protocal.h"
#include "judgeThread.h"
#include "clientInfo.h"
#include "makeconfig.h"
#include "stringUtil.h"
#include <sys/select.h>
#include <map>

MySQLManager db;

int main(int argc,char **argv) {
    daemon_init();
    if(chdir(XJOJHOME) < 0) {
        BLOG("can't change dir to XJOJHOME", SYSCALL_ERROR);
        return -1;
    }

    if(write_pid("server.pid") < 0) {
        return 1;
    }

    Config *config = Config::getInstant();

    if(config->isIniError()) {
        LOG(ERROR) << config->getErrorMesg();
        return 1;
    }

    Log::getInstance(config->getLogRoot() + config->getLogFileName());

	try {
		connectMysql();
	} catch(Error& err) {
        BLOG(err.toString(), ERROR);
		return 1;
	}

    db.execute("update status set poolStatus=0 where judgeStatus='Waiting' and poolStatus=1");
    db.execute("update status set judgeStatus='Waiting',poolStatus='0',`time`=0,memory=0,`case`=0 where judgeStatus='Judging' or judgeStatus='Running' or judgeStatus='Compiling'");

    //start the taskProducer thread
    TaskProducer *taskProducer = new TaskProducer;
    taskProducer->start();

    //create a tcp server, and start listenning
    int port = config->getListenPort();
    int listen_socket = CreateServerSocket(&port);
    if(listen_socket <= 0)
        return 1;

    BLOG(combineString("judge server is listening on port ", port), INFO);

    int clientInfoPort = config->getClientInfoPort();
    int client_info_listen_fd = CreateServerSocket(&clientInfoPort);
    if(client_info_listen_fd < 0)
        return 1;

    BLOG(combineString("client info server is listening on port ", clientInfoPort), INFO);

    int fd;
    if((fd = open("/dev/null", O_RDWR, 0600)) == -1
        || dup2(fd, 0) == -1
        || dup2(fd, 1) == -1
        || dup2(fd, 2) == -1)
        return 1;

    struct sockaddr_in un;
    socklen_t len = sizeof(un);
    char ipbuf[16];

    int max_fd = listen_socket;
    if(max_fd < client_info_listen_fd)
        max_fd = client_info_listen_fd;

    while(1) {
        fd_set fd_read;
        FD_ZERO(&fd_read);
        FD_SET(listen_socket, &fd_read);
        FD_SET(client_info_listen_fd, &fd_read);

        DLOG(INFO) << "select\n";

        if(select(max_fd + 1, &fd_read, 0, 0, 0) < 0) {
            if(errno == EINTR)
                continue;

            BLOG("main(): select error", SYSCALL_ERROR);
            return 1;
        }

        DLOG(INFO) << "select ok\n";

        if(FD_ISSET(listen_socket, &fd_read)) {
            //a judge client is connecting
            int client_socket = accept(listen_socket, (struct sockaddr*)&un, &len);
            if(client_socket < 0) {
                BLOG("main(): accept error", SYSCALL_ERROR);
                continue;
            }

            port = ntohs(un.sin_port);
            inet_ntop(AF_INET, &un.sin_addr, ipbuf, sizeof(ipbuf));

            JudgeThread *judgeThread = new JudgeThread(client_socket, true);
            addClientInfo(judgeThread, port, ipbuf, formatTime("%m-%d %H:%M:%S"));
            judgeThread->start();
        }

        if(FD_ISSET(client_info_listen_fd, &fd_read)) {
            int connfd = accept(client_info_listen_fd, (struct sockaddr*)&un, &len);
            if(connfd < 0) {
                BLOG("main(): accept error", SYSCALL_ERROR);
                continue;
            }

            std::string info = getClientInfo();
            writen(connfd, info.c_str(), info.size());
            close(connfd);
        }
    }

    taskProducer->join();
    return 0;
}

