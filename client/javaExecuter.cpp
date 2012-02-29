/*
 * javaExecuter.cpp
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
#include "config.h"
#include "util.h"
#include "stringUtil.h"
#include "javaExecuter.h"

#include <unistd.h>
#include <sys/wait.h>

JavaExecuter::~JavaExecuter() {

}

void JavaExecuter::run() {
    init();

    std::string workingDir = combineString(Config::getInstant()->getTmpUserPath(), DIR_SEPARTOR, runId);
   
    int port = 0, status;
    int server_sock = CreateServerSocket(&port);
    struct sockaddr_un un;
    socklen_t len = sizeof(un);

    if(server_sock < 0) {
        BLOG("JavaExecuter::run(): create server sock error", SYSCALL_ERROR);
        this->_result = SERVER_ERROR;
        return;
    }

    pid_t pid = fork();

    if(pid < 0) {
        BLOG("JavaExecuter::run() fork error", SYSCALL_ERROR);
        this->_result = SERVER_ERROR;
        return;
    }

    if(pid == 0) {
        close(server_sock);

        if(chdir(workingDir.c_str()) == -1) {
            LLOG("Fail to change working dir to " + workingDir, SYSCALL_ERROR);
            raise(SIGKILL);
            return;
        }
        
        std::string rootPath = Config::getInstant()->getRootPath();
        if(stdinFileName[0] != '/' && stdinFileName[0] != '.') {
            stdinFileName = rootPath + stdinFileName;
        }
        if(stdoutFileName[0] != '/' && stdoutFileName[0] != '/') {
            stdoutFileName = rootPath + stdoutFileName;
        }

        std::string minMem = combineString("-Xms", limitMemory / 2 + 200, 'k');
        std::string maxMem = combineString("-Xmx", limitMemory + 200, 'k');
        std::string javaLib = combineString("-Djava.library.path=", rootPath, "bin"); 
        std::string jarPath = combineString(rootPath, "bin/JavaSandbox.jar");
        std::string portStr = combineString(port);
        std::string limitTimeStr = combineString(limitTime);
        std::string limitMemoryStr = combineString(limitMemory);
        std::string limitOutputStr = combineString(limitOutput);
        std::string uidStr = combineString(Config::getInstant()->getJobUID());
        std::string gidStr = combineString(Config::getInstant()->getJobGID());

        for(int i = 0; i < 100; i++) close(i);

        execlp("java",
                minMem.c_str(),
                maxMem.c_str(),
                javaLib.c_str(),
                "-jar",
                jarPath.c_str(),
                portStr.c_str(),
                limitTimeStr.c_str(),
                limitMemoryStr.c_str(),
                limitOutputStr.c_str(),
                uidStr.c_str(),
                gidStr.c_str(),
                stdinFileName.c_str(),
                stdoutFileName.c_str(),
                NULL);

        LLOG("JavaExecuter()::run() Fail to run java", SYSCALL_ERROR);
        exit(-1);
    } else {
        //parent
        while(1) {
            //the accept function will be interuppted by the SIGCHLD signal
            int client_sock = accept(server_sock, (struct sockaddr*)&un, &len);

            if(client_sock < 0) {
                if(errno != EINTR) {
                    BLOG("JavaExecuter::run() fail to accept", SYSCALL_ERROR);
                    close(server_sock);
                    this->_result = SERVER_ERROR;
                    return;
                }else if(waitpid(pid, &status, WNOHANG) > 0) {
                    break;
                }
            } else {
                unsigned int _time, _memory;
                while(ReadUint32(client_sock, &_time) >= 0 &&
                    ReadUint32(client_sock, &_memory) >= 0) {
                    this->_runnedTime = _time;
                    this->_runnedMemory = _memory;
                    judgeThread->updateRunInfo(_time, _memory);
                }
                close(client_sock);
                while(waitpid(pid, &status, 0) < 0 && errno != ECHILD) {
                
                }
                break;
            }
        }
        close(server_sock);

        if(WIFSIGNALED(status)) {
            switch(WTERMSIG(status)) {
                case SIGXCPU:
                    this->_result = TIME_LIMIT_EXCEEDED;
                    break;
                default:
                    DLOG(ERROR) << "Java process was terminated by signal " << WTERMSIG(status);
                    this->_result = RUNTIME_ERROR;
            }
        } else {
            this->_result = WEXITSTATUS(status);
            if(this->_result == 1) {
                this->_result = SERVER_ERROR;
            }else{
                if(this->_result)
                    this->_result += 1000;
            }

            if(this->_result == 0) {
                this->_result = TraceProcess::NORMAL;
                if(this->_runnedMemory > limitMemory) {
                    this->_result = MEMORY_LIMIT_EXCEEDED;
                }

                if(this->_runnedTime > limitTime) {
                    this->_result = TIME_LIMIT_EXCEEDED;
                }
            }
        }
    }
}

