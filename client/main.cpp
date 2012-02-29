/*
 * main.cpp
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

#include "log.h"
#include "util.h"
#include "stringUtil.h"
#include "config.h"
#include "file.h"
#include "judgeThread.h"
#include "makeconfig.h"
#include <getopt.h>
#include <vector>

int main(int argc,char *argv[]) {
    daemon_init();
    if(chdir(XJOJHOME) < 0) {
        BLOG("can't change dir to XJOJHOME", SYSCALL_ERROR);
        return 1;
    }

    if(write_pid("judge.pid") < 0) {
        return 1;
    }

    Config *config = Config::getInstant();
    Log::getInstance(config->getLogRoot() + config->getLogFileName());
    if(config->isIniError()) {
        BLOG(config->getErrorMesg(), ERROR);
        return 1;
    }

    if(!config->initConfig(argc, argv)) {
        BLOG(config->getErrorMesg(), ERROR);
        return 1;
    }

    removeDir(Config::getInstant()->getTmpUserPath().c_str(), false);

    int totThread = Config::getInstant()->getThreadNumber();
    if(totThread <= 0) {
        BLOG("Thread number is less or equal than 0", ERROR);
        return 1;
    }
    
    int fd;
    if((fd = open("/dev/null", O_RDWR, 0600)) == -1
        || dup2(fd, 0) == -1
        || dup2(fd, 1) == -1
        || dup2(fd, 2) == -1)
        return 1;

    // Write data to a closed socket will cause SIGPIPE signal
    installSignalHandler(SIGPIPE, SIG_IGN, 0);

    std::string host = Config::getInstant()->getServer().first;
    int port = Config::getInstant()->getServer().second;

    std::vector<JudgeThread *> threads(totThread);
    for(int i = 0; i < totThread; i++) {
        threads[i] = new JudgeThread(i + 1);
        threads[i]->setServerAddress(host);
        threads[i]->setPort(port);
    }

    for(int i = 0; i < totThread; i++) {
        threads[i]->start();
    }

    for(int i = 0; i < totThread; i++) {
        threads[i]->join();
    }

    return 0;
}

