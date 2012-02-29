/*
 * traceProcess.cpp
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

#include "traceProcess.h"
#include "util.h"
#include "log.h"
#include "disabled_syscall.h"
#include "makeconfig.h"
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/user.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <vector>

const int TraceProcess::NORMAL = -1;

int TraceProcess::getRunningTime() {
#ifndef HAVE_WAIT4
    char buffer[64];
    sprintf(buffer, "/proc/%d/stat", this->pid);
    FILE* fp = fopen(buffer, "r");

    if (fp == NULL) {
        return this->_runnedTime;
    }

    int utime, stime;
    while (fgetc(fp) != ')');
    fgetc(fp);
    if (fscanf(fp, "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %d %d", &utime, &stime) < 2) {
        fclose(fp);
        return this->_runnedTime;
    }
    fclose(fp);

    static int clktck = 0;
    if (clktck == 0) {
        clktck = sysconf(_SC_CLK_TCK);
    }
    this->_runnedTime = int((utime + stime + 0.0) / clktck * 1000);
#else
	if(getusage) {
		long runnedTime = process_usage.ru_utime.tv_sec * 1000000 + process_usage.ru_utime.tv_usec
						+ process_usage.ru_stime.tv_sec * 1000000 + process_usage.ru_stime.tv_usec;
		this->_runnedTime = runnedTime / 1000;
	}
#endif

    return this->_runnedTime;
}

int TraceProcess::getRunningMemory() {
    char buffer[64];
    sprintf(buffer, "/proc/%d/status", this->pid);
    FILE* fp = fopen(buffer, "r");
    if (fp == NULL) {
   	    return this->_runnedMemory;
    }

    int vmPeak = 0, vmSize = 0, vmExe = 0, vmLib = 0;

    while (fgets(buffer, 32, fp)) {
   	    if(!strncmp(buffer, "VmPeak:", 7)) {
      	    sscanf(buffer + 7, "%d", &vmPeak);
        }else if (!strncmp(buffer, "VmSize:", 7)) {
             sscanf(buffer + 7, "%d", &vmSize);
        }else if (!strncmp(buffer, "VmExe:", 6)) {
             sscanf(buffer + 6, "%d", &vmExe);
        }else if (!strncmp(buffer, "VmLib:", 6)) {
             sscanf(buffer + 6, "%d", &vmLib);
        }
    }

    fclose(fp);

    if (vmPeak) {
   	    vmSize = vmPeak;
    }

    this->_runnedMemory = vmSize - vmExe -vmLib;
    return this->_runnedMemory;
}

void TraceProcess::getdata(long addr, char *str, int len) {
    union u {
        long val;
        char chars[LONG_SIZE];
    }data;

    int i = 0;
    while( i < len) {
        data.val = ptrace(PTRACE_PEEKDATA, pid, addr + i, NULL);
        for(int j = 0; j < LONG_SIZE; j++) {
            if(data.chars[j] && i + j < len)
                str[i + j] = data.chars[j];
            else {
                str[i + j] = 0;
                return;
            }
        }
        i += LONG_SIZE;
    }
    str[len] = 0;
}

int TraceProcess::createProcess(std::string cmd, const TraceProcessInfo& info) {
    init();

    std::string filename[] = {info.stdinFileName,
                          info.stdoutFileName,
                          info.stderrFileName};

    this->_exit = false;

	std::vector<std::string> vargv;
	std::istringstream iss(cmd, std::istringstream::in);
	std::string arg;
	while(iss >> arg) {
		vargv.push_back(arg);
	}

	char **argv = new char*[vargv.size() + 1];
	argv[vargv.size()] = 0;
	char * dupProgName = strdup(vargv[0].c_str());
	argv[0] = dupProgName;
	for(int i = 1; i < vargv.size(); i++) {
		argv[i] = strdup(vargv[i].c_str());
	}

    this->pid = fork();

	if(this->pid != 0) {
		for(int i = 0; i < vargv.size(); i++)
			free(argv[i]);
		delete []argv;
	}

    if(this->pid < 0) {
        BLOG("TraceProcess::createProcess(): Fail to fork a process", SYSCALL_ERROR);
        this->_result = SERVER_ERROR;
        return -1;
    } else if(this->pid > 0) {
        //parent
        return this->pid;
    }

    int mode[] = {O_RDONLY,
                O_RDWR | O_CREAT | O_TRUNC,
                O_RDWR};

    int fd[] = {info.fdStdin,
                info.fdStdout,
                info.fdStderr
               };

    int mask = S_IRUSR | S_IWUSR | S_IROTH;
    umask(0);

    for(int i = 0; i < 3; i++) {
        if(filename[i].length() > 0) {
            fd[i] = open(filename[i].c_str(), mode[i], mask);
            if(fd[i] == -1) {
                LLOG("TraceProcess::createProcess(): Fail to open " + filename[i], SYSCALL_ERROR);
                
                for(int j = 0; j < i; j++) {
                    if(filename[i].length() > 0) {
                        close(fd[j]);
                    }
                }
                
                raise(SIGKILL);
            }
        }
    } 

    for(int i = 0; i < 3; i++) {
        if(fd[i]) {
            if(dup2(fd[i], i) == -1) {
                LLOG("TraceProcess::createProcess(): Fail to dup a fd", SYSCALL_ERROR);
                raise(SIGKILL);
            }
        }
    }

    for(int i = 3; i < 100; i++) {
        close(i);
    }

    if(info.timeLimit) {
        int t = info.timeLimit / 1000;
        if(info.timeLimit % 1000) {
            t++;
        }

        if(setLimit(RLIMIT_CPU, t) == -1) {
            LLOG("TraceProcess::createProcess(): Fail to set cpu limit", SYSCALL_ERROR);
            raise(SIGKILL);
        }
    }

    if(info.memoryLimit) {
        if(setLimit(RLIMIT_DATA, info.memoryLimit * 1024) == -1) {
            LLOG("TraceProcess::createProcess(): Fail to set memory limit", SYSCALL_ERROR);
            raise(SIGKILL);
        }
    }

    if(info.outputLimit) {
        if(setLimit(RLIMIT_FSIZE, info.outputLimit * 1024) == -1) {
            LLOG("TraceProcess::createProcess(): Fail to set output limit", SYSCALL_ERROR);
            raise(SIGKILL);
        }
    }

    if(info.fileNoLimit) {
        if(setLimit(RLIMIT_NOFILE, info.fileNoLimit) == -1) {
            LLOG("TraceProcess::createProcess(): Fail to set fileno limit", SYSCALL_ERROR);
            raise(SIGKILL);
        } 
    }

    if(info.procNoLimit) {
        if(setLimit(RLIMIT_NPROC, info.procNoLimit) == -1) {
            LLOG("TraceProcess::createProcess(): Fail to set procno limit", SYSCALL_ERROR);
            raise(SIGKILL);
        }
    }

    if(info.uid) {
        if(setuid(info.uid) == -1) {
            LLOG(combineString("TraceProcess::createProcess(): Fail to set uid to ", info.uid), SYSCALL_ERROR);
            raise(SIGKILL);
        }
    }

    if(info.gid) {
        if(setgid(info.gid) == -1) {
            LLOG(combineString("TraceProcess::createProcess(): Fail to set gid to ", info.gid), SYSCALL_ERROR);
            raise(SIGKILL);
        }
    }

    if(setLimit(RLIMIT_CORE, 0) == -1) {
        LLOG("TraceProcess::createProcess(): Fail to set core limit to 0", SYSCALL_ERROR);
        raise(SIGKILL);
    }

    if(info.workingDir.size() > 0) {
        if(chdir(info.workingDir.c_str()) == -1) {
            LLOG("TraceProcess::createProcess(): Fail to change woring dir to " + info.workingDir, SYSCALL_ERROR);
            raise(SIGKILL);
        }
    }
    
    if(info.trace) {
        if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            LLOG("TraceProcess::createProcess(): Fail to trace child process", SYSCALL_ERROR); 
            raise(SIGKILL);
        }
    }

    if(execvp(dupProgName, argv) == -1) {
        LLOG("TraceProcess::createProcess(): Fail to execute commmand " + cmd, SYSCALL_ERROR);
        raise(SIGKILL);
    }

    exit(-1);
    return 0;
}

bool TraceProcess::canOpen(const char *path, int mode) {
    std::string sp(path);

    if((mode & O_WRONLY) == O_WRONLY ||
        (mode & O_RDWR) == O_RDWR ||
        (mode & O_CREAT) == O_CREAT ||
        (mode & O_APPEND) == O_APPEND) {
        BLOG("TraceProcess::canOpen(): open " + sp + " is not allowed", WARNING);
        return false;
    }

    if(sp.size() == 0) {
        DLOG(WARNING) << "TraceProcess::canOpen(): can't open an empty file\n";
        return false;
    }

    return true;
}

void TraceProcess::traceMe() {
    struct user_regs_struct regs;

    DLOG(INFO) << pid << " trace_me\n";

    //the waitpid function will be interuppted by the SIGCHLD signal
    int wret;
#ifdef HAVE_WAIT4
	while((wret = wait4(this->pid, &status, 0, &process_usage)) > 0) {
		getusage = true;
#else
    while((wret = waitpid(this->pid, &status, 0)) > 0) {
#endif
        //DLOG(INFO) << pid << " waitpid return " << wret << "\n";

        if(WIFSIGNALED(status)) {
            DLOG(INFO) << "TraceProcess()::traceMe(): child was killed by signal\n";

            _exit = true;
            if(WTERMSIG(status) == SIGKILL && this->_result == -1) {
                this->_result = RUNTIME_ERROR;
            }
            break;
        }

        if(!WIFSTOPPED(status)) {
            //if(WEXITSTATUS(status)) {
            //    this->_result = RUNTIME_ERROR;
            //}
            _exit = true;
            break;
        }

        int sig = WSTOPSIG(status);
        if(sig != SIGTRAP) {
            DLOG(INFO) << "TraceProcess()::traceMe(): caught a signal " << sig << "\n";

            getRunningTime();
            getRunningMemory();

            switch(sig) {
            case SIGFPE:
                DLOG(INFO) << "TraceProcess()::traceMe(): SIGFPE\n";

                this->_result = FLOATING_POINT_ERROR;
                break;

            case SIGXCPU:
                DLOG(INFO) << "TraceProcess()::traceMe(): SIGXCPU\n";

                this->_result = TIME_LIMIT_EXCEEDED;
                break;

            case SIGBUS:
            case SIGSEGV:
                DLOG(INFO) << "TraceProcess()::traceMe(): SIGSEGV\n";

                this->_result = SEGMENTATION_FAULT;
                break;

            case SIGXFSZ:
                DLOG(INFO) << "TraceProcess()::traceMe(): SIGXFSZ\n";

                this->_result = OUTPUT_LIMIT_EXCEEDED;
                break;

            case SIGILL:
            case SIGKILL:
                DLOG(INFO) << "TraceProcess()::traceMe(): SIGKILL\n";

                if(this->_result == -1) {
                    this->_result = RUNTIME_ERROR;
                }
                break;

            case SIGALRM:
                ;
            }

            ptrace(PTRACE_SYSCALL, this->pid, NULL, sig);
            continue;
        }

        ptrace(PTRACE_GETREGS, this->pid, 0, &regs);
        
        switch(regs.ORIG_EAX) {
        case SYS_exit:
        case SYS_exit_group:
            DLOG(INFO) << "TraceProcess()::traceMe(): child sys_exit\n";

            getRunningTime();
            getRunningMemory();
            break;

        case SYS_execve:
            if(!first_exec) {
                first_exec = true;
                DLOG(INFO) << pid << " execve, ptrace it\n";

                ptrace(PTRACE_SYSCALL, this->pid, NULL, NULL);
                continue;
            }
            break;

        case SYS_brk:
            if(!insyscall) {
               brk = (unsigned long)regs.EBX;
               insyscall = true;
            }else{
                if(((unsigned long)regs.EAX) < brk) {
                    DLOG(INFO) << "TraceProcess()::traceMe(): brk request " << brk << " return " << regs.EAX << "\n";

                    this->_result = MEMORY_LIMIT_EXCEEDED;
                    ptrace(PTRACE_KILL, this->pid, 0, 0);
                    continue;
                }
                insyscall = false;
            }
            break;
        
        case SYS_open:
            if(!insyscall) {
                char path[FILENAME_MAX + 1];
                insyscall = true;
                getdata(regs.EBX, path, sizeof(path));
				DLOG(INFO) << "open file:" << path << "\n";
                if(!canOpen(path, regs.ECX)) {
					regs.ORIG_EAX = SYS_exit;
					regs.EBX = 1;
					ptrace(PTRACE_SETREGS, this->pid, 0, &regs);
                    this->_result = RUNTIME_ERROR;
					break;
                }
            }else{
                insyscall = false;
            }
            ptrace(PTRACE_SYSCALL, this->pid, 0, 0);
            continue;
        }

        if(regs.ORIG_EAX < sizeof(disabled_syscall) &&
            disabled_syscall[regs.ORIG_EAX]) {

            DLOG(INFO) << "TraceProcess()::traceMe(): Restricted syscall " << regs.ORIG_EAX << "\n";

            this->_result = RESTRICTED_FUNCTION;
            getRunningTime();
            getRunningMemory();

			regs.ORIG_EAX = SYS_exit;
			regs.EAX = SYS_exit;
			regs.EBX = 1;
			ptrace(PTRACE_SETREGS, this->pid, NULL, &regs);
            ptrace(PTRACE_KILL, this->pid, 0, 0);
			continue;
        }else{
            ptrace(PTRACE_SYSCALL, this->pid, 0, 0);
        }

        ptrace(PTRACE_SYSCALL, this->pid, NULL, NULL);
    }
}

