/*
 * judgeThread.h
 *
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

#ifndef  JUDGETHREAD_INC
#define  JUDGETHREAD_INC

#include "thread.h" 
#include "socket.h" 
#include <string>
#include <sstream>

class JudgeThread : public Thread {
public:
    JudgeThread(int threadId);
    ~JudgeThread();
    
    void run();

    std::string getThreadName() const {
        return this->threadName; 
    }

    void setServerAddress(const std::string& serverAddress) {
        this->serverAddress = serverAddress;
    }

    void setPort(int port) {
        this->port = port;
    }

    bool readReply();

    bool getSourceCode(std::string filename);

    bool syncData();

private:
    void startJudging();

    int writeLine(const std::ostream& sin);

    bool getCompilerInfo();

    void clearComilerInfo();

public:
    void updateRunInfo(int _time, int _memory);

private:
    Socket *tcpSocket;

    int threadId;

    std::string threadName;
    std::string serverAddress;
    int port;

	int runId;
	int proId;
    std::string compileCmd;
    std::string execCmd;
    std::string ext;
	int time;
	int memory;
	int casenum;
	bool spj;

    int totTime;
    int maxMemory;
};

#endif   /* ----- #ifndef JUDGETHREAD_INC  ----- */

