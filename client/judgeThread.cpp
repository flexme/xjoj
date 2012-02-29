/*
 * judgeThread.cpp 
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

#include "judgeThread.h"
#include "log.h"
#include "compiler.h"
#include "sqlError.h"
#include "executer.h"
#include "normalExecuter.h"
#include "javaExecuter.h"
#include "defaultJudge.h"
#include "specialJudge.h"
#include "compileError.h"
#include "socket.h"
#include "stringUtil.h"
#include "file.h"
#include "mutex.h"
#include "../server/protocal.h"
#include "downloadFile.h"
#include <map>
#include <set>

struct CompilerInfo {
    std::string compileCmd;
    std::string execCmd;
    std::string ext;

    CompilerInfo(const std::string& ccmd, const std::string& ecmd, const std::string ext)
        : compileCmd(ccmd), execCmd(ecmd), ext(ext) {
        
        }
};

static std::map<int, CompilerInfo*> compilerInfo;
static Mutex compilerMutex;
static Mutex sync_data_mutex;
static bool has_sync_data;

JudgeThread::JudgeThread(int threadId) {
    this->threadId = threadId;
    this->threadName = combineString("judge thread ", threadId);
    this->tcpSocket = NULL;
}

JudgeThread::~JudgeThread() {
    if(tcpSocket) {
        delete tcpSocket;
    }
}

void JudgeThread::run() {
    //this->setPriority(Thread::TimeCriticalPriority);
    //
    if(tcpSocket) {
        delete tcpSocket;
        tcpSocket = NULL;
    }

    tcpSocket = new Socket();
    bool ret = tcpSocket->connect(this->serverAddress, this->port);
    if(!ret) {
        BLOG(threadName + ":JudgeThread::run(): connect error", ERROR);
        return;
    }

    tcpSocket->setNonBlock();

    BLOG(threadName + " connect to server!", INFO);

    int wn = writeLine(std::ostringstream() << AUTH << ' ' << Config::getInstant()->getPassword() << '\n');
    if(wn == -1) {
        BLOG(threadName + " JudgeThread()::run() socket write error", ERROR);
        tcpSocket->close();
        return; 
    }

    if(!getCompilerInfo())
        return;

    if(Config::getInstant()->needToSyncData()) {
        sync_data_mutex.lock();
        if(!has_sync_data) {
            syncData();
            has_sync_data = true;
        }
        sync_data_mutex.unlock();
    }

    startJudging();
}

int JudgeThread::writeLine(const std::ostream& sin) {
    const std::ostringstream& osin = dynamic_cast<const std::ostringstream&>(sin);
    std::string str = osin.str();
    int wn = this->tcpSocket->write(str.c_str(), str.size());
    return wn;
}

void JudgeThread::startJudging() {
    DLOG(INFO) << threadName << " JudgeThread::startJudging()\n";

    while(1) {
        if(this->tcpSocket->status() != Socket::Connected) {
            break;
        }

        runId = -1;
        int ret = writeLine(std::ostringstream() << GET_TASK << '\n');
        if(ret == -1) {
            break; 
        }

        if(!readReply())
            break;

        if(runId == -1) {
            continue;
        }

        DLOG(INFO) << threadName << " compiling runId: " << runId << "\n";

        ret = writeLine(std::ostringstream() << UPDATE_STATUS << ' ' << COMPILING << '\n');
        if(ret == -1) {
            break;
        }

        bool ok = true;

        Compiler compiler(compileCmd, ext, runId, this);
        try {
            compiler.run();
        } catch (SqlError& err) {
            BLOG(threadName + " " + err.toString(), ERROR);
            ret = writeLine(std::ostringstream() << UPDATE_STATUS << ' ' << COMPILATION_ERROR << '\n');
            if(ret == -1) {
                break;
            }
            ok = false;
        } catch (CompileError& err) {
            std::string ce = err.toString();
            ret = writeLine(std::ostringstream() << UPDATE_COMERROR << ' ' << ce.size() << '\n' << ce);
            if(ret == -1) {
                break; 
            }
            ok = false;
        }

        if(!ok) {
            continue;
        }

        totTime = 0;
        maxMemory = 0;
        Judge* judge = NULL;

        DLOG(INFO) << threadName << " running runId:" << runId << "\n";

        ret = writeLine(std::ostringstream() << UPDATE_STATUS << ' ' << RUNNING << '\n');
        if(ret == -1) {
            break; 
        }

        int result = ACCEPTED;

        Executer *executer = NULL;

        if(ext == "java") {
            time *= 5;
            executer = new JavaExecuter(runId, this);
        }else{
			replace(execCmd, "{:basename}", combineString(runId));
            executer = new NormalExecuter(runId, execCmd);
        }

        executer->setLimitOutput(65535);
        executer->setLimitMemory(memory);

        for(int i = 1; i <= casenum; i++) {
            ret = writeLine(std::ostringstream() << UPDATE_CASE << ' ' << i << '\n');

            DLOG(INFO) << threadName << " running case " << i << "\n";
            
            executer->setStdinFileName(Config::getInstant()->getDataIn(proId, i));
            executer->setStdoutFilename(Config::getInstant()->getUserOut(runId));
            executer->setLimitTime((int)time - totTime);
            
            executer->run();

            DLOG(INFO) << threadName << " time:" << executer->getRunningTime() << "\n"; 

            totTime += executer->getRunningTime();
            int nm = executer->getRunningMemory();
            if(maxMemory < nm) {
                maxMemory = nm;
            }

            if(executer->getResult() != TraceProcess::NORMAL) {
                DLOG(INFO) << threadName << " case " << i << "occour error:" << executer->getResult() << "\n";

                result = executer->getResult();
                ::unlink(Config::getInstant()->getUserOut(runId).c_str());
                break;
            }

            if(spj) {
                judge = new SpecialJudge(Config::getInstant()->getUserOut(runId),
                                        Config::getInstant()->getDataIn(proId, i),
                                        Config::getInstant()->getDataOut(proId, i),
                                        Config::getInstant()->getSpjFile(proId));
            } else{
                judge = new DefaultJudge(Config::getInstant()->getUserOut(runId),
                                    Config::getInstant()->getDataOut(proId, i));
            }

            ret = judge->judge();
            delete judge;


            if(ret != ACCEPTED) {
                DLOG(INFO) << threadName << " case " << i << " result:" << ret << "\n";
                DLOG(INFO) << threadName << " total time:" << totTime << "\n";

                result = ret;
                break;
            }
        }

        if(executer) {
            delete executer;
            executer = NULL;
        }

        if(result == TIME_LIMIT_EXCEEDED && totTime < time) {
            totTime = time + 1;
        }

        if(result == MEMORY_LIMIT_EXCEEDED && maxMemory < memory) {
            maxMemory = memory + 1;
        }

        //CLOG((threadName + " judging result: " + QString::number(result)).toAscii().data(), INFO);
        ret = writeLine(std::ostringstream() << UPDATE_STATUS << ' ' << result << '\n');
        if(ret == -1) {
            break; 
        }

        ret = writeLine(std::ostringstream() << UPDATE_RUNINFO << ' ' << totTime << ' ' << maxMemory << '\n');
        if(ret == -1) {
            break; 
        }
    }

    tcpSocket->close();
}

bool JudgeThread::readReply() {
    char buf[1024];

    int ret = tcpSocket->readLine(buf);
    if(ret == -1)
        return false;

    buf[ret] = 0;

    std::istringstream sin(buf, std::istringstream::in);
    int oper;
    sin >> oper;
    
    if(oper == REPLY_TASK) {
        sin >> runId;
        if(runId == -1) {
            return true;
        }

        //langId used to choose a compiler
        int langId;
        sin >> proId >> time >> memory >> casenum >> spj >> langId;

        if(compilerInfo.count(langId) == 0) {
            BLOG(combineString(threadName, ":there is no such compiler, compiler id:", langId), ERROR);
            execCmd = "";   //server error
            return true;
        }

        CompilerInfo *cinfo = compilerInfo[langId];
        compileCmd = cinfo->compileCmd;
        execCmd = cinfo->execCmd;
        ext = cinfo->ext;

        DLOG(INFO) << threadName << "get task:runID:" << runId << ",proId:" << proId << ",time:" << time << ",memory:" << memory << ",spj:"<< spj << "\n";
    }

    return true;
}

void JudgeThread::updateRunInfo(int _time, int _memory) {
    int t_time = totTime + _time;
    int t_memory = maxMemory > _memory ? maxMemory : _memory;
    int ret = writeLine(std::ostringstream() << UPDATE_RUNINFO << ' ' << t_time << ' ' << t_memory << '\n');
    if(ret == -1) {
        tcpSocket->close();
        terminate();
    }
}

bool JudgeThread::getSourceCode(std::string filename) {
    int wn = writeLine(std::ostringstream() << GET_SOURCECODE << '\n');
    if(wn == -1) {
        BLOG("JudgeThread::getSourceCode() socket write error", ERROR);
        tcpSocket->close();
        return false;
    }

    return downloadFile(tcpSocket, filename);
}

bool JudgeThread::getCompilerInfo() {
    compilerMutex.lock();

    if(compilerInfo.size() > 0) {
        compilerMutex.unlock();
        return true;
    }

    int wn = writeLine(std::ostringstream() << GET_COMPILERINFO << '\n');
    if(wn == -1) {
        BLOG("JudgeThread::getCompilerInfo() socket write error", ERROR);
        tcpSocket->close();
        compilerMutex.unlock();
        return false;
    }

    char buf[1024];
    int compilerNumber = 0;

    wn = tcpSocket->readLine(buf);
    if(wn <= 0) 
        goto displayError;

    compilerNumber = atoi(buf);

    for(int i = 0; i < compilerNumber; i++) {
        wn = tcpSocket->readLine(buf);
        if(wn < 0)
            goto displayError;
        int id = atoi(buf);

        wn = tcpSocket->readLine(buf);
        if(wn < 0)
            goto displayError;
        std::string compileCmd(buf);

        wn = tcpSocket->readLine(buf);
        if(wn < 0)
            goto displayError;
        std::string execCmd(buf);

        wn = tcpSocket->readLine(buf);
        if(wn < 0)
            goto displayError;
        std::string ext(buf);

        if(compilerInfo.count(id) > 0)
            delete compilerInfo[id];

        compilerInfo[id] = new CompilerInfo(compileCmd, execCmd, ext);
    }

    compilerMutex.unlock();
    return true;

displayError:
    BLOG("JudgeThread()::getCompilerInfo() socket read error", ERROR);
    compilerMutex.unlock();
    tcpSocket->close();
    clearComilerInfo();
    return false;
}

void JudgeThread::clearComilerInfo() {
    for(std::map<int, CompilerInfo *>::iterator it = compilerInfo.begin();
        it != compilerInfo.end();
        it++) {
        delete it->second;
    }

    compilerInfo.clear();
}

bool JudgeThread::syncData() {
    BLOG("starting to sync data", INFO);

    int totProblems;
    Config *config = Config::getInstant();
    std::map<int, std::pair<int, int> > dataNeedToSync;
    std::set<int> spjNeedToSync;

    int wn = writeLine(std::ostringstream() << DATA_SYNC << '\n');
    if(wn < 0)
        goto errorSocket;

    char buf[1025];
    wn = tcpSocket->readLine(buf);
    if(wn < 0)
        goto errorSocket;

    totProblems = atoi(buf);

    for(int i = 0; i < totProblems; i++) {
        wn = tcpSocket->readLine(buf);
        if(wn < 0)
            goto errorSocket;

        int proId, casenum, spj;
        sscanf(buf, "%d %d %d", &proId, &casenum, &spj);

        std::string dataInDir = combineString(config->getDataInPath(), proId);
        std::string dataOutDir = combineString(config->getDataOutPath(), proId);
        if(access(dataInDir.c_str(), R_OK) != 0) {
            if(mkdir(dataInDir.c_str(), 0777) < 0) {
                BLOG("mkdir " + dataInDir, SYSCALL_ERROR);
                tcpSocket->close();
                return false;
            }
        }

        if(access(dataOutDir.c_str(), R_OK) != 0) {
            if(mkdir(dataOutDir.c_str(), 0777) < 0) {
                BLOG("mkdir " + dataOutDir, SYSCALL_ERROR);
                tcpSocket->close();
                return false;
            }
        }

        int curCaseNum = 0;
        while(1) {
            if(access(config->getDataIn(proId, curCaseNum + 1).c_str(), R_OK) == 0)
                curCaseNum++;
            else
                break;
        }

        if(curCaseNum < casenum) {
            dataNeedToSync[proId].first = casenum;
            dataNeedToSync[proId].second = curCaseNum;
        }

        if(spj) {
            if(access(config->getSpjFile(proId).c_str(), R_OK) != 0)
                spjNeedToSync.insert(proId);
        }
    }

    for(std::map<int, std::pair<int, int> >::iterator it = dataNeedToSync.begin();
        it != dataNeedToSync.end();
        it++) {
        int proId = it->first;
        int casenum = it->second.first;
        int curCaseNum = it->second.second;
        for(int i = curCaseNum + 1; i <= casenum; i++) {
            sprintf(buf, "getting data for proId:%d, case:%d...............", proId, i);
            BLOG(buf, INFO);
            writeLine(std::ostringstream() << GET_DATA << ' ' << proId << ' ' << i << '\n');
            downloadFile(tcpSocket, config->getDataIn(proId, i), true);
            downloadFile(tcpSocket, config->getDataOut(proId, i), true);
            sprintf(buf, "getting data for proId:%d, case:%d. OK!", proId, i);
            BLOG(buf, INFO);
        }
    }

    for(std::set<int>::iterator it = spjNeedToSync.begin();
        it != spjNeedToSync.end();
        it++) {
        int proId = *it;
        sprintf(buf, "getting spj for proId:%d...............", proId);
        BLOG(buf, INFO);
        writeLine(std::ostringstream() << GET_DATA << ' ' << proId << ' ' << '0' << '\n');
        downloadFile(tcpSocket, config->getSpjFile(proId), true);
        sprintf(buf, "getting spj for proId:%d. OK!", proId);
        BLOG(buf, INFO);
    }

    BLOG("sync data successfully!", INFO);

    return true;

errorSocket:
    BLOG("JudgeThread::syncData() socket error", ERROR);
    tcpSocket->close();
    return false;
}

