/*
 * normalExecuter.cpp
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

#include "normalExecuter.h"
#include "traceProcess.h"
#include "sqlError.h"
#include "judgeStatus.h"
#include "log.h"
#include "config.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

NormalExecuter::~NormalExecuter() {

}

void NormalExecuter::updateStatus() {
    this->_runnedTime = p.getRunningTime();
    this->_runnedMemory = p.getRunningMemory();
    this->_result = p.getResult();
    if(this->_runnedTime > limitTime) {
        this->_result = TIME_LIMIT_EXCEEDED;
    }
    if(this->_result == TIME_LIMIT_EXCEEDED && this->_runnedTime <= limitTime) {
        this->_runnedTime = limitTime + 1;
    }
    if(this->_runnedMemory > limitMemory) {
        this->_result = MEMORY_LIMIT_EXCEEDED;
    }
    if(this->_result == MEMORY_LIMIT_EXCEEDED && this->_runnedMemory <= limitMemory) {
        this->_runnedMemory = limitMemory + 1;
    }

    DLOG(INFO) << "time:" << _runnedTime << " memory:" << _runnedMemory << "\n";
}

void NormalExecuter::run() {
	TraceProcessInfo pinfo;
	pinfo.stdinFileName = stdinFileName;
	pinfo.stdoutFileName = stdoutFileName; 
	pinfo.stderrFileName = "";
	pinfo.uid = Config::getInstant()->getJobUID();
	pinfo.gid = Config::getInstant()->getJobGID();
	pinfo.timeLimit = limitTime;
	pinfo.memoryLimit = limitMemory;
	pinfo.outputLimit = limitOutput;
	pinfo.procNoLimit = 1;
	//pinfo.fileNoLimit = 4;
    pinfo.trace = true;
    pinfo.workingDir = combineString(Config::getInstant()->getTmpUserPath(), runId);

    init();

    //installSignalHandler(SIGALRM, sigalrm_handler, 0);

    if(p.createProcess(cmd, pinfo) == -1) {
        this->_result = p.getResult();
        return;
    }

    DLOG(INFO) << "child pid: " << p.getPid() << "\n";
   
    for(;;) {
        if(p.isExited()) {
            break;
        }else{
            updateStatus(); 
        }

        if(_result != TraceProcess::NORMAL) {
            ::kill(p.getPid(), SIGKILL);
        }

        p.traceMe();
    }

    if(this->_result == TraceProcess::NORMAL) {
        this->_result = p.getResult();
    }
    updateStatus();
}

