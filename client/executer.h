/*
 * executer.h
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

#ifndef __EXECUTER_H
#define __EXECUTER_H

#include "util.h"
#include "traceProcess.h"

class Executer {
public:
	Executer(int runId, const std::string& stdinFileName, const std::string& stdoutFileName,
				int limitTime, int limitMemory, int limitOutput):
            runId(runId),
			stdinFileName(stdinFileName),
			stdoutFileName(stdoutFileName),
			limitTime(limitTime),
            limitMemory(limitMemory),
			limitOutput(limitOutput) {
                init();
            }

    Executer(int runId): runId(runId) {
        init();
    }

	virtual ~Executer();

	virtual void run() = 0;

    int getResult() {
        return _result;
    }

    int getRunningMemory() {
        return this->_runnedMemory;
    }

    int getRunningTime() {
        return this->_runnedTime;
    }

    void setStdinFileName(const std::string& filename) {
        stdinFileName = filename;
    }

    void setStdoutFilename(const std::string& filename) {
        stdoutFileName = filename;
    }

    void setLimitTime(int limitTime) {
        this->limitTime = limitTime;
    }

    void setLimitMemory(int limitMemory) {
        this->limitMemory = limitMemory;
    }

    void setLimitOutput(int limitOutput) {
        this->limitOutput = limitOutput;
    }

private:
    Executer(const Executer&);

    Executer& operator=(const Executer&);
    const Executer& operator=(const Executer&) const;

protected:
    void updateStatus();

    void init();

protected:
    int _result;
    int _runnedTime;
    int _runnedMemory;
    int runId;

    std::string stdinFileName;
    std::string stdoutFileName;
    int limitTime;
    int limitMemory;
    int limitOutput;
};
#endif

