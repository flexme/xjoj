/*
 * judgeThread.h
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

#ifndef __JUDGE_THREAD_H
#define __JUDGE_THREAD_H

#include "connectmysql.h"
#include "../client/judgeStatus.h"
#include "util.h"
#include "config.h"
#include "socket.h"
#include "log.h"
#include "protocal.h"
#include "thread.h"
#include <string>

class JudgeThread : public Thread {
public:
    JudgeThread(int socket_fd, bool block) {
        auth = false;
        client_socket = socket_fd;
        tcpSocket = new Socket(socket_fd, block);
        runId = "-1";
        hasUpdateStatus = false;
    }

    ~JudgeThread() {
        delete tcpSocket;
    }

    void run();

    std::string getRunId() const {
        return runId;
    }

    void operator()(const unsigned char* buf, int len);

private:
    void restoreStatus();

    void disconnected();

    void do_get_compilerinfo();

    void do_update_status(int status);

    void do_get_task();

    void do_update_runinfo(const std::string& time, const std::string& memory);

    void do_update_case(const std::string& casenum);

    void do_update_compilererror(int errorMesgLen);

    void do_get_sourcecode();

    void do_sync_data();

    void do_get_data(int proId, int casenum);

    bool send_file(std::string filename);

private:
    bool auth;

    int client_socket;

    Socket *tcpSocket;

    std::string runId;

    bool hasUpdateStatus;

};

#endif

