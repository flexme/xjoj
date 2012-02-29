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

#include "clientInfo.h"
#include "mutex.h"
#include <map>
#include <sstream>

static std::map<const JudgeThread*, ClientInfo*> clientInfos;
static Mutex clientInfoMutex;

void addClientInfo(const JudgeThread* judgeThread, int port, std::string host, std::string connectTime) {
    if(clientInfos.count(judgeThread) > 0)
        return;

    ClientInfo *info = new ClientInfo;
    info->port = port;
    info->host = host;
    info->connectTime = connectTime;
    clientInfos[judgeThread] = info;
}

void delClientInfo(const JudgeThread * judgeThread) {
    clientInfoMutex.lock();
    if(clientInfos.count(judgeThread) > 0) {
        delete clientInfos[judgeThread];
        clientInfos.erase(judgeThread);
    }
    clientInfoMutex.unlock();
}

std::string getClientInfo() {
    std::ostringstream sin; 
    clientInfoMutex.lock();
    for(std::map<const JudgeThread*, ClientInfo*>::iterator it = clientInfos.begin();
        it != clientInfos.end();
        it++) {
        sin << it->second->host << '|' << it->second->port << '|' << it->second->connectTime
            << '|' << it->first->getRunId() << '\n';
    }
    clientInfoMutex.unlock();

    return sin.str();
}

