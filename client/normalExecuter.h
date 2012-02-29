/*
 * normalExecuter.h 用来执行普通的程序的类
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

#ifndef  NORMALEXECUTER_INC
#define  NORMALEXECUTER_INC

#include "executer.h"
#include "traceProcess.h"
#include <string>

class NormalExecuter : public Executer {
public:
    NormalExecuter(int runId, const std::string& cmd, const std::string& stdinFileName, const std::string& stdoutFileName,
        int limitTime, int limitMemory, int limitOutput) :
        Executer(runId, stdinFileName, stdoutFileName, limitTime, limitMemory, limitOutput),
        cmd(cmd)
    {
    
    }

    NormalExecuter(int runId, const std::string& cmd) : 
        Executer(runId), cmd(cmd) {
        
    }

    void run();

    ~NormalExecuter();

private:
    void updateStatus();

private:
    std::string cmd;
    TraceProcess p;    
};

#endif   /* ----- #ifndef NORMALEXECUTER_INC  ----- */

