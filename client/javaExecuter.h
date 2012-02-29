/*
 * javaExecuter.h
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

#ifndef  JAVAEXECUTER_INC
#define  JAVAEXECUTER_INC

#include "executer.h"
#include "judgeThread.h"

class JavaExecuter : public Executer {
public:
    JavaExecuter(int runId, const std::string& stdinFileName, const std::string& stdoutFilename, 
                int limitTime, int limitMemory, int limitOutput, 
                JudgeThread* judgeThread):
                Executer(runId, stdinFileName, stdoutFilename, 
                        limitTime, limitMemory, limitOutput),
                judgeThread(judgeThread)
    {
        
    }

    JavaExecuter(int runId, JudgeThread* judgeThread):
        Executer(runId),
        judgeThread(judgeThread)
    {
    
    }

    void run();
    
    ~JavaExecuter();

private:
    JudgeThread* judgeThread;
};

#endif   /* ----- #ifndef JAVAEXECUTER_INC  ----- */

