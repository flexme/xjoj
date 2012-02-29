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

#include "process.h"
#include "mutex.h"
#include "thread.h"
#include "file.h"
#include "log.h"
#include <map>
#include <signal.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/time.h>

static int process_deadChild_pipe[2];
static void (*sa_old_sigchld_handler)(int) = 0;
static void sa_sigchild_handler(int signum) {
    DLOG(INFO) << "sigchild\n";
    nonBlockWriten(process_deadChild_pipe[1], "", 1);
    DLOG(INFO) << "sigchild writen ok\n";

    if(sa_old_sigchld_handler && sa_old_sigchld_handler != SIG_IGN)
        sa_old_sigchld_handler(signum);
}

struct ProcessInfo {
    Process *process;
    int deathPipe;
    pid_t pid;
};

class ProcessManager : public Thread {
public:
    ProcessManager();
    ~ProcessManager();

    void run();
    void catchDeadChildren();
    void add(pid_t pid, Process *process);
    void remove(Process *process);
    void lock();
    void unlock();

private:
    Mutex mutex;
    std::map<Process*, ProcessInfo*> children;
};

static Mutex* processManagerGlobalMutex() {
    static Mutex mutex;
    return &mutex;
}

static ProcessManager *processManager() {
     // The constructor of QProcessManager should be called only once
     Mutex *mutex = processManagerGlobalMutex();
     MutexLocker locker(mutex);
     static ProcessManager processManager;
     return &processManager;
}

ProcessManager::ProcessManager() {
    DLOG(INFO) << "ProcessManager::ProcessManager()\n";
    
    safe_pipe(process_deadChild_pipe, O_NONBLOCK);

    struct sigaction oldAction;
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = sa_sigchild_handler;
    action.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &action, &oldAction);
    if(oldAction.sa_handler != sa_sigchild_handler) {
        sa_old_sigchld_handler = oldAction.sa_handler;
    }
}

ProcessManager::~ProcessManager() {
    DLOG(INFO) << "ProcessManager::~ProcessManager()\n";

    //kill remainer child processes
    std::map<Process*, ProcessInfo*>::iterator it = children.begin();
    while(it != children.end()) {
        ProcessInfo *info = it->second;
        ::kill(info->pid, SIGKILL);
        delete info;
        it++;
    }
    //...
}

void ProcessManager::run() {
    while(true) {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(process_deadChild_pipe[0], &readset);

        DLOG(INFO) << "ProcessManager::run() waiting for children to die\n";

        int nselect = select(process_deadChild_pipe[0] + 1, &readset, 0, 0, 0);

        if(nselect < 0) {
            if(errno == EINTR)
                continue;

            BLOG("ProcessManager::run() select error:", SYSCALL_ERROR);
            break;
        }

        char c;
        if(nonBlockReadn(process_deadChild_pipe[0], &c, 1) < 0 || c == '@') {
            BLOG("ProcessManager::run() readn error:", SYSCALL_ERROR);
            break;
        }

        catchDeadChildren();
    }
}

void ProcessManager::catchDeadChildren() {
    MutexLocker locker(&mutex);
    
    std::map<Process*, ProcessInfo*>::iterator it = children.begin();
    while(it != children.end()) {
        ProcessInfo *info = it->second;
        writen(info->deathPipe, "", 1);

        DLOG(INFO) << "ProcessManager::run() sending death notice to " << info->process << "\n";

        it++;
    }
}

void ProcessManager::add(pid_t pid, Process * process) {
    //this function will be only called in Process::start when the mutex is already locked
    //so now we cannot lock mutex again

    DLOG(INFO) << "ProcessManager::add() adding pid " << pid << " process " << process << "\n";

    ProcessInfo *info = new ProcessInfo;
    info->process = process;
    info->deathPipe = process->deathPipe[1];
    info->pid = pid;

    if(children.count(process) > 0)
        return;

    children[process] = info;
}

void ProcessManager::remove(Process *process) {
    MutexLocker locker(&mutex);

    DLOG(INFO) << "ProcessManager::remove() removing pid " << process->_pid << " process " << process << "\n";

    if(children.count(process) > 0) {
        ProcessInfo *info = children[process];
        delete info;
        children.erase(process);
    }
}

void ProcessManager::lock() {
    mutex.lock();
}

void ProcessManager::unlock() {
    mutex.unlock();
}

static void create_pipe(int pipe[2]) {
    if(pipe[0] != -1) {
        close(pipe[0]);
        pipe[0] = -1;
    }

    if(pipe[1] != -1) {
        close(pipe[1]);
        pipe[1] = -1;
    }

    if(safe_pipe(pipe) != 0)
        BLOG("Process: cannot create_pipe", SYSCALL_ERROR);
}

Process::Process() {
    _pid = 0;
    _exitCode = 0;
    _exitStatus = Process::NormalExit;
    deathPipe[0] = -1;
    deathPipe[1] = -1;
    stdoutChannel[0] = -1;
    stdoutChannel[1] = -1;
    stderrChannel[0] = -1;
    stderrChannel[1] = -1;
    _crashed = false;
    dying = false;
    steup_func = NULL;
}

Process::~Process() {
    if(!dying) {
        if(!_processDied()) {
            kill();
        } 
    }
    cleanup();
    processManager()->remove(this);
}

void Process::cleanup() {
    DLOG(INFO) << "Process::cleanup()\n";

    _pid = 0;
    //close file handler
    destroyPipe(deathPipe);
    destroyPipe(stdoutChannel);
    destroyPipe(stderrChannel);
}

void Process::destroyPipe(int pipe[2]) {
    if(pipe[1] != -1) {
        close(pipe[1]);
        pipe[1] = -1;
    }

    if(pipe[0] != -1) {
        close(pipe[0]);
        pipe[0] = -1;
    }
}

bool Process::_processDied() {
    DLOG(INFO) << "Process::_processDied()\n";

    if(dying)
        return true;

    if(!waitForDeadChild())
        return false;

    dying = true;

    processManager()->remove(this);

    if(_crashed) {
        _exitStatus = Process::CrashExit;
    }

    cleanup();

    DLOG(INFO) << "Process::_processDied() process is dead\n";

    return true;
}

std::string Process::workingDirectory() const {
    return _workingDirectory;
}

void Process::setWorkingDirectory(const std::string& dir) {
    _workingDirectory = dir;
}

pid_t Process::pid() const {
    return _pid;
}

bool Process::waitForDeadChild() {
    DLOG(INFO) << "Process::waitForDeadChild()\n";

    char c;
    nonBlockReadn(deathPipe[0], &c, 1);

    int exitStatus;
    while(1) {
        if(waitpid(_pid, &exitStatus, WNOHANG) > 0) {
            processManager()->remove(this);
            _crashed = !WIFEXITED(exitStatus);
            _exitCode = WEXITSTATUS(exitStatus);

            stdoutString = readString(stdoutChannel[0]);
            stderrString = readString(stderrChannel[0]);

            DLOG(INFO) << "Process::waitForDeadChild dead with exitCode " << _exitCode << ", crashed?" << _crashed << "\n";

            return true;
        }else{
            if(errno == EINTR)
                continue;
            else
                return false;
        }
    }

    return false;
}

void Process::kill() {
    DLOG(INFO) << "Process:kill() pid: " << _pid << "\n";

    if(_pid)
        ::kill(_pid, SIGKILL);
}

static int select_msec(int nfds, fd_set *fdread, fd_set *fdwrite, int timeout) {
    if(timeout < 0)
        return select(nfds, fdread, fdwrite, 0, 0);

    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    return select(nfds, fdread, fdwrite, 0, &tv);
}

static int timeval_subtract(struct timeval *x, struct timeval *y, struct timeval *result) {
    int nsec;

    result->tv_sec = 0;
    result->tv_usec = 0;

    if(x->tv_sec > y->tv_sec)
        return -1;

    if(x->tv_sec == y->tv_sec && x->tv_usec > y->tv_usec)
        return -1;

    result->tv_sec = y->tv_sec - x->tv_sec;
    result->tv_usec = y->tv_usec - x->tv_usec;

    if(result->tv_usec < 0) {
        result->tv_sec--;
        result->tv_usec += 1000000;
    }

    return 0;
}

static inline void add_fd(int &nfds, int fd, fd_set *fdset) {
    FD_SET(fd, fdset);
    if((fd) > nfds)
        nfds = fd;
}

bool Process::waitForFinished(int msec) {
    while(1) {
        fd_set fdread;
        int nfds = -1;

        FD_ZERO(&fdread);

        if(deathPipe[0] != -1 && !dying)
            add_fd(nfds, deathPipe[0], &fdread);

        struct timeval start, stop, diff;
        gettimeofday(&start, 0);

        int ret = select_msec(nfds + 1, &fdread, 0, msec);
        if(ret < 0) {
            if(errno != EINTR) {
                //error occur
                break;
            }
        }

        if(ret == 0) {
            DLOG(INFO) << "Process::waitForFinished() timeout\n";
            return false;
        }

        if(deathPipe[0] == -1 || FD_ISSET(deathPipe[0], &fdread)) {
            if(_processDied())
                return true;
        }

        gettimeofday(&stop, 0);
        timeval_subtract(&start, &stop, &diff);
        int take = diff.tv_sec * 1000 + diff.tv_usec / 1000;
        if(msec > 0 && msec <= take) {
            break;
        }

        msec -= take;
    }

    return false;
}

bool Process::start(const std::string& program) {
    processManager()->start();

    create_pipe(deathPipe);
    create_pipe(stdoutChannel);
    create_pipe(stderrChannel);

    stdoutString = "";
    stderrString = "";

    std::vector<std::string> vargv;
    std::istringstream iss(program, std::istringstream::in);
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

    processManager()->lock();

    pid_t childPid = fork();
    int lastForkErrno = errno;
    if(childPid != 0) {
        //parent process
        //clean up duplicated memory
        for(int i = 0; i < vargv.size(); i++)
            free(argv[i]);
        delete []argv;
    }

    if(childPid < 0) {
        errno = lastForkErrno;
        BLOG("Process::start() fork() failed:", SYSCALL_ERROR);

        processManager()->unlock();
        cleanup();
        return false;
    }

    if(childPid == 0) {
        //child process
        //exec

        if(steup_func)
            (*steup_func)();

        if(_workingDirectory.size() > 0) {
            chdir(_workingDirectory.c_str());
        }

        close(stdoutChannel[0]);
        close(stdoutChannel[1]);
        destroyPipe(deathPipe);

        if(stdoutChannel[1] != -1)
            ::dup2(stdoutChannel[1], fileno(stdout));

        if(stderrChannel[1] != -1)
            ::dup2(stderrChannel[1], fileno(stderr));

        execvp(dupProgName, argv);
        ::exit(-1);
    }

    processManager()->add(childPid, this);
    _pid = childPid;
    processManager()->unlock();

    if(stdoutChannel[1] != -1) {
        ::close(stdoutChannel[1]);
        stdoutChannel[1] = -1;
    }

    if(stderrChannel[1] != -1) {
        ::close(stderrChannel[1]);
        stderrChannel[1] = -1;
    }

    if(stdoutChannel[0] != -1)
        ::fcntl(stdoutChannel[0], F_SETFL, ::fcntl(stdoutChannel[0], F_GETFL) | O_NONBLOCK);

    if(stderrChannel[0] != -1)
        ::fcntl(stderrChannel[0], F_SETFL, ::fcntl(stderrChannel[0], F_GETFL) | O_NONBLOCK);

    ::fcntl(deathPipe[0], F_SETFL, ::fcntl(deathPipe[0], F_GETFL) | O_NONBLOCK);

    return true;
}

std::string Process::readString(int fd) {
    char buf[1025];
    std::string ret;
    buf[0] = 0;

    while(true) {
        int nread = nonBlockReadn(fd, buf, 1024);
        if(nread <= 0)
            break;

        buf[nread] = 0;
        ret.append(buf);
    }

    return ret;
}

std::string Process::readAllStandardOut() {
    //_processDied();
    return stdoutString;
}

std::string Process::readAllStandardError() {
    //_processDied();
    return stderrString;
}

Process::ExitStatus Process::exitStatus() {
    //_processDied();
    return _exitStatus;
}

int Process::exitCode() {
    //_processDied();
    return _exitCode;
}

