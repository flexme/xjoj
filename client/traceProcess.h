/*
 * traceProcess.h
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

#ifndef  __TRACEPROCESS_INC
#define  __TRACEPROCESS_INC

#include "judgeStatus.h"
#include "config.h"
#include "makeconfig.h"
#include <string>
#include <sys/wait.h>
#include <sys/resource.h>

#define LONG_SIZE sizeof(long)

#if __WORDSIZE == 64
#define ORIG_EAX orig_rax
#define EAX rax
#define EBX rbx
#define ECX rcx
#define EDX rdx
#else
#define ORIG_EAX orig_eax
#define EAX eax
#define EBX ebx
#define ECX ecx
#define EDX edx
#endif

struct TraceProcessInfo {
    TraceProcessInfo():
        fdStdin(0),
        fdStdout(0),
        fdStderr(0),
        stdinFileName(),
        stdoutFileName(),
        uid(0),
        gid(0),
        timeLimit(0),
        memoryLimit(0),
        outputLimit(0),
        procNoLimit(0),
        fileNoLimit(0),
        trace(true) {

        }

    int fdStdin, fdStdout, fdStderr;

    std::string stdinFileName;

    std::string stdoutFileName;

    std::string stderrFileName;

    std::string workingDir;

    int uid, gid;

    int timeLimit;

    int memoryLimit;

    int outputLimit;

    int procNoLimit;

    int fileNoLimit;

    bool trace;
};

class TraceProcess {
private:
    pid_t pid;

    bool _exit;

    bool insyscall;

    bool first_exec;

    int status;

    unsigned long brk;

    JudgeResult _result;

    int _runnedTime;

    int _runnedMemory;

#ifdef HAVE_WAIT4
	struct rusage process_usage;
	bool getusage;
#endif

private:
    void getdata(long addr, char *str, int len);

    bool canOpen(const char *path, int mode);

    void init() {
        _exit = false;
        insyscall = false;
        first_exec = false;
        _result = NORMAL;
        _runnedMemory = 0;
        _runnedTime = 0;
#ifdef HAVE_WAIT4
		getusage = false;
#endif
    }

public:
    static const int NORMAL;

    TraceProcess() {
        init();
    }

    pid_t getPid() const {
        return pid;
    }

    bool isExited() const {
        return _exit;
    }

    JudgeResult getResult() const {
        return this->_result;
    }

    int getRunningTime();

    int getRunningMemory();

    int createProcess(std::string cmd, const TraceProcessInfo& info);

    void traceMe();
};

#endif   /* ----- #ifndef PROCESS_INC  ----- */

