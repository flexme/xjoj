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

#ifndef  PROCESS_H
#define  PROCESS_H

#include <unistd.h>
#include <signal.h>
#include <string>

typedef void (*child_steup_func)();

class ProcessManager;

class Process {
public:
    enum ExitStatus {
        NormalExit,
        CrashExit
    };

public:
    explicit Process();
    virtual ~Process();

    bool start(const std::string& program);

    std::string workingDirectory() const;
    void setWorkingDirectory(const std::string& dir);

    pid_t pid() const;

    int exitCode();
    Process::ExitStatus exitStatus();

    void kill();

    bool waitForFinished(int msec);

    std::string readAllStandardOut();

    std::string readAllStandardError();

    void setChildSteupFunc(child_steup_func func) {
        steup_func = func;
    }

    friend class ProcessManager;

private:
    pid_t _pid;
    std::string _workingDirectory;
    std::string _program;

    std::string stdoutString;
    std::string stderrString;

    bool dying;

    int deathPipe[2];
    int stdoutChannel[2];
    int stderrChannel[2];
    
    void destroyPipe(int pipe[2]);
    std::string readString(int fd);

    bool waitForDeadChild();
    bool _processDied();

    int _exitCode;
    Process::ExitStatus _exitStatus;
    bool _crashed;

    void cleanup();

    child_steup_func steup_func;

    Process(const Process&);
    Process& operator=(const Process&);
    const Process& operator=(const Process&) const;
};

#endif   /* ----- #ifndef PROCESS_H  ----- */

