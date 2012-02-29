/*
 * specialJudge.cpp
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

#include "specialJudge.h"
#include "judgeStatus.h"
#include "process.h"
#include "sqlError.h"
#include "util.h"
#include "log.h"
#include "config.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sstream>

JudgeResult SpecialJudge::judge() {
    if(access(judgeFileName.c_str(), R_OK) != 0) {
        BLOG("cann't find the spj file : " + judgeFileName, ERROR);
        return SERVER_ERROR;
    }

    if(access(stdInFile.c_str(), R_OK) != 0) {
        BLOG("can't find the data file : " + stdInFile, ERROR);
        return SERVER_ERROR;
    }

    if(access(userOut.c_str(), R_OK) != 0) {
        BLOG("can't find the data file : " + userOut, ERROR);
        return SERVER_ERROR;
    }

    Process sp;
    std::stringstream sin;
    sin << judgeFileName << ' ' << stdInFile << ' ' << userOut << ' ' << stdOutFile;

    if(!sp.start(sin.str())) {
        BLOG("cann't start the spj program " + judgeFileName, SYSCALL_ERROR);
        return SERVER_ERROR;
    }

    if(!sp.waitForFinished(3000)) {
        BLOG("spj cann't stoped " + judgeFileName, ERROR);  
        sp.kill();
        return SERVER_ERROR;
    }

    if(sp.exitStatus() == Process::CrashExit) {
        BLOG("spj crashed " + judgeFileName, ERROR);
        return SERVER_ERROR;
    }

    int ret = sp.exitCode();
    if(ret == 0) {
        return ACCEPTED;
    } else if(ret == 1) {
        return WRONG_ANSWER;
    } else if(ret == 2) {
        return PRESENTATION_ERROR;
    } else {
        return SERVER_ERROR;
    }

    return sp.exitCode();

    /*
    ProcessInfo info;
	info.stdinFileName = stdFile;
    info.stderrFileName = userOut;
	info.uid = Config::getInstant()->getJobUID();
    info.gid = Config::getInstant()->getJobGID();
	info.timeLimit = 10;
    info.memoryLimit = 65535;
	info.outputLimit = 16;
	info.trace = false;
    info.workingDir = "";

    Process p;
	pid_t pid = p.createProcess(judgeFileName, info);
	if(pid == -1) {
        return p.getResult();
    }

	int status;
	if (waitpid(pid, &status, 0) < 0) {
   	    pid = 0;
        BLOG("can not wait pid by program " + judgeFileName, ERROR);
        return SERVER_ERROR;
    }

	if(WIFSIGNALED(status)) {
        BLOG("Spj terminated by signal " + QString::number(WTERMSIG(status)), ERROR);
        return SERVER_ERROR;
    }

	switch(WEXITSTATUS(status)) {
    case 0:
        return ACCEPTED;
    case 2:
        return PRESENTATION_ERROR;
    default:
        return WRONG_ANSWER;
    }

    return SERVER_ERROR;
    */
}

