/*
 * judgeThread.cpp
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
#include "taskProducer.h"
#include "file.h"
#include "clientInfo.h"
#include "compress.h"
#include <cstdlib>

//when status of previous task has not been updated, and is requesting a 
//new task, we must first restore the previous task
void JudgeThread::restoreStatus() {
    if(runId == "-1" || hasUpdateStatus)
        return;

    sqlMutex.lock();
    db.execute("update status set judgeStatus='Waiting', poolStatus=0,`time`=0,memory=0,`case`=0 where runId=" + runId);
    sqlMutex.unlock();
}

void JudgeThread::disconnected() {
    BLOG("a client disconnected", INFO);
    restoreStatus();
    tcpSocket->close();
    delClientInfo(this);
}

//return the compiler list to a client
void JudgeThread::do_get_compilerinfo() {
    sqlMutex.lock(); 
    db.executeResult("select id, compileCmd, executeCmd, ext from language");
    std::vector<std::vector<std::string> > result = db.getResult();
    sqlMutex.unlock();

    std::ostringstream sin;
    sin << result.size() << "\n";
    for(int i = 0; i < result.size(); i++) {
        sin << result[i][0] << "\n" << result[i][1] << "\n" 
            << result[i][2] << "\n" << result[i][3] << "\n";
    }
    const std::string str = sin.str();
    if(tcpSocket->write(str.c_str(), str.size()) == -1) {
        tcpSocket->close();
    }
}

// send the source code to the client side
void JudgeThread::do_get_sourcecode() {
    sqlMutex.lock();
    db.executeResult(combineString("select code from source_code where runId=", runId));
    std::vector<std::vector<std::string> > result = db.getResult();
    db.freeResult();
    sqlMutex.unlock();

    std::string &code = result[0][0];
    std::ostringstream sin;
    sin << code.size() << '\n' << code;
    const std::string str = sin.str();
    if(tcpSocket->write(str.c_str(), str.size()) == -1) {
        tcpSocket->close();
    }
}

void JudgeThread::operator()(const unsigned char *buf, int len) {
    if(len > 0) {
        std::ostringstream sin;
        sin << len << '\n';
        std::string str = sin.str();

        if(tcpSocket->write(str.c_str(), str.size()) == -1) {
            tcpSocket->close();
            return;
        }

        if(tcpSocket->write((const char *)buf, len) == -1) {
            tcpSocket->close();
            return;
        }
    }
}

// send a file to the client side. The file will be compressed.
bool JudgeThread::send_file(std::string filename) {
    int fsize = ::fileSize(filename.c_str());
    //std::ostringstream sin;
    //sin << fsize << '\n';
    //const std::string str = sin.str();
    //tcpSocket->write(str.c_str(), str.size());

    if(fsize <= 0)
        //file is not exists
        return true;

    /*int fd = ::open(filename.c_str(), O_RDONLY);
    
    char buf[4096];
    while(1) {
        int readcount = ::readn(fd, buf, sizeof(buf));
        if(readcount < 0) {
            BLOG("JudgeThread::send_file() readn from file error", ERROR);
            tcpSocket->close();
            ::close(fd);
            return false;
        }
        if(readcount == 0)
            break;

        tcpSocket->write(buf, readcount);
    }

    ::close(fd);
    */

    compress(filename.c_str(), this);
    tcpSocket->write("0\n", 2);
    return true;
}

// Send the test case data to the client
void JudgeThread::do_get_data(int proId, int casenum) {
    if(casenum == 0) {
        // send the SPJ (Sepcial Judgement) program
        send_file(Config::getInstant()->getSpjFile(proId));
        return;
    }

    std::string filename = Config::getInstant()->getDataIn(proId, casenum);
    if(!send_file(filename))
        return;

    filename = Config::getInstant()->getDataOut(proId, casenum);
    if(!send_file(filename))
        return;
}

// Response to the sync requresting from the client
void JudgeThread::do_sync_data() {
    std::string sql = "select problemId,spj from problem order by problemId";
    sqlMutex.lock();
    db.executeResult(sql);
    std::vector<std::vector<std::string> > result = db.getResult();
    db.freeResult();
    sqlMutex.unlock();

    std::ostringstream sin;
    // the number of total problems
    sin << result.size() << '\n';

    Config *config = Config::getInstant();
    for(int i = 0; i < result.size(); i++) {
        int casenum = 0;
        int proId = atoi(result[i][0].c_str());
        while(1) {
            if(access(config->getDataIn(proId, casenum + 1).c_str(), R_OK) == 0)
                casenum++;
            else
                break;
        }
        sin << proId << ' ' << casenum << ' ' << result[i][1] << '\n';
    }

    const std::string str = sin.str();
    if(tcpSocket->write(str.c_str(), str.size()) == -1) {
        tcpSocket->close();
    }
}

//update the judging status
void JudgeThread::do_update_status(int status) {
    std::string result;

    hasUpdateStatus = true;

    switch(status) {
		case JUDGING: 					result = "Judging";hasUpdateStatus = false;         break;
		case COMPILING: 				result = "Compiling";hasUpdateStatus = false;		break;
		case RUNNING:					result = "Running";hasUpdateStatus = false;		    break;
		case RUNTIME_ERROR:			    result = "Runtime Error";		                    break;
		case ACCEPTED:					result = "Accepted";			                    break;
		case WRONG_ANSWER:			    result = "Wrong Answer";		                    break;
		case TIME_LIMIT_EXCEEDED:	    result = "Time Limit Exceeded";	                    break;
		case MEMORY_LIMIT_EXCEEDED:     result = "Memory Limit Exceeded";                   break;
		case OUTPUT_LIMIT_EXCEEDED:     result = "Output Limit Exceeded";                   break;
		case COMPILATION_ERROR:		    result = "Compilation Error"; 	                    break;
		case PRESENTATION_ERROR:	    result = "Presentation Error";	                    break;
		case FLOATING_POINT_ERROR:      result = "Floating Point Error";                    break;
		case SEGMENTATION_FAULT:	    result = "Runtime Error(SEGMENT)";                  break;
		case READY:						result = "Ready";hasUpdateStatus = false;           break;
		case SERVER_ERROR:			    result = "Server Error";		                    break;
        case RESTRICTED_FUNCTION:       result = "Restricted Function";                     break;
		default:						result = "Unknown Error";		                    break;
	}

    sqlMutex.lock();
    db.execute("update status set judgeStatus='" + result + "' where runId=" + runId);
    sqlMutex.unlock();
}

//return a judge task to a client
//if there is no judge task, the thread will be blocked
void JudgeThread::do_get_task() {
    restoreStatus();
    runId = "-1";

    full.acquire();

    mutex.lock();
    std::stringstream sin;
    sin << REPLY_TASK << " ";
    if(tasks.size() == 0) {
        sin << -1 << "\n";
    }else{
        Task task = tasks.front();
        tasks.pop_front();
        sin << task.runId << ' ' << task.proId << ' ' << task.time << ' '
            << task.memory << ' ' << task.casenum << ' ' << task.spj << ' '
            << task.langId << "\n" ;

        runId = task.runId;
        do_update_status(JUDGING);
        hasUpdateStatus = false;
    }
    mutex.unlock();

    std::string str = sin.str();
    if(tcpSocket->write(str.c_str(), str.size()) == -1)
        tcpSocket->close();
}

//update the running time and running memory
void JudgeThread::do_update_runinfo(const std::string& time, const std::string& memory) {
    std::string sql("update status set `time`=");
    sql.append(time);
    sql.append(", `memory`=");
    sql.append(memory);
    sql.append(" where runId=");
    sql.append(runId);

    sqlMutex.lock();
    db.execute(sql);
    sqlMutex.unlock();
}

//update the number of running case
void JudgeThread::do_update_case(const std::string& casenum) {
    std::string sql("update status set `case`=");
    sql.append(casenum);
    sql.append(" where runId=");
    sql.append(runId);

    sqlMutex.lock();
    db.execute(sql);
    sqlMutex.unlock();
}

//update the compiler error
//errorMesgLen is the length of error message
void JudgeThread::do_update_compilererror(int errorMesgLen) {
    char *buf = new char[errorMesgLen + 1];
    tcpSocket->read(buf, errorMesgLen);
    buf[errorMesgLen] = 0;

    DLOG(INFO) << buf << "\n";
    
    std::string errmsg(buf);
    delete buf;
    replaceAll(errmsg, "'", "\\\'");
    std::string sql("update status set `errInfo`='");
    sql.append(errmsg);
    sql.append("' where runId=");
    sql.append(runId);

    sqlMutex.lock();
    db.execute(sql);
    sqlMutex.unlock();
    
    do_update_status(COMPILATION_ERROR);
}

//a client has requested, so serve it
void JudgeThread::run() {
    char buf[1024];

    tcpSocket->setNonBlock();

    while(1) {
        int wn = tcpSocket->readLine(buf);
        if(wn < 0) {
            disconnected();
            return;
        }

        DLOG(INFO) << buf << "\n";

        int oper;
        std::istringstream sin(buf);
        sin >> oper;
        if(!auth) {
            if(oper != AUTH) {
                disconnected();
                return;
            }

            std::string ps;
            sin >> ps;
            if(ps != Config::getInstant()->getPassword()) {
                //wrong password
                disconnected();
                return;
            }

            auth = true;
            continue;
        }

        DLOG(INFO) << "command:" << oper << "\n";
        
        std::string time, memory;
        std::string casenum;
        int status;
        int errorMesgLen;
        int get_data_proId, get_data_casenum;

        switch(oper) {
            case GET_COMPILERINFO:
                do_get_compilerinfo();
                break;

            case GET_TASK:
                do_get_task();
                break;

            case UPDATE_STATUS:
                sin >> status;
                do_update_status(status);
                break;

            case UPDATE_RUNINFO:
                sin >> time >> memory;
                do_update_runinfo(time, memory);
                break;

            case UPDATE_CASE:
                sin >> casenum;
                do_update_case(casenum);
                break;

            case UPDATE_COMERROR:
                sin >> errorMesgLen;
                do_update_compilererror(errorMesgLen);
                break; 

            case GET_SOURCECODE:
                do_get_sourcecode();
                break;

            case DATA_SYNC:
                do_sync_data();
                break;

            case GET_DATA:
                sin >> get_data_proId >> get_data_casenum;
                do_get_data(get_data_proId, get_data_casenum);
                break;
        }
    }
}

