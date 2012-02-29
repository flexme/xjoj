/*
 * compiler.cpp
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

#include "compiler.h"
#include "compileError.h"
#include "sqlError.h"
#include "log.h"
#include "config.h"
#include "process.h"
#include "util.h"
#include "file.h"
#include "stringUtil.h"
#include <sys/resource.h>
#include <unistd.h>

// set the resource limitation for the compiler program
static void steupChildProcess() {
    setLimit(RLIMIT_CPU, 10);
    setLimit(RLIMIT_AS,  250 * 1024 * 1024);
}

void Compiler::run() throw(CompileError, SqlError) {
    std::string tmpUserPath = Config::getInstant()->getTmpUserPath();
    tmpUserPath = combineString(tmpUserPath, runId);

    if(mkdir(tmpUserPath.c_str(), 0777) < 0) {
        throw SqlError(combineString("Failed to create tmp dir for runId", runId));
        return;
    }

    tmpUserPath.push_back(DIR_SEPARTOR);

    codeFile = combineString(runId, '.', sourceExt);

    std::string file = Config::getInstant()->getTmpSourcePath() + codeFile;

	if(access(file.c_str(), R_OK) != 0) {
        if(!parent->getSourceCode(file)) {
            throw SqlError("Failed to get source code from server"); 
            return;
        }

        if(access(file.c_str(), R_OK) != 0) {
    		throw SqlError("Failed to find the source file " + file);
	    	return;
        }
	}

    std::string src_file = combineString(tmpUserPath, codeFile);

    if(rename(file.c_str(), src_file.c_str()) < 0) {
        throw SqlError(combineString("Cannot move source file for runId ", runId));
		return;
    }

	if(compileCmd.size() == 0)
		return;

    replace(compileCmd, "{:basename}", combineString(runId));

    replace(compileCmd, "{:mainfile}", codeFile);
    
	Process compileProcess;
    compileProcess.setChildSteupFunc(steupChildProcess);
    
    if(sourceExt == "java") {
        std::string javaCodeFile = combineString(tmpUserPath, "Main.java");

        if(rename(src_file.c_str(), javaCodeFile.c_str()) < 0) {
            throw SqlError(combineString("Cannot move java source file for runId ", runId));
            return;
        }
    }
    compileProcess.setWorkingDirectory(tmpUserPath);

	if(!compileProcess.start(compileCmd)) {
		throw SqlError(combineString("Failed to start the compiling process where runId=", runId));
		return;
	}

	if(!compileProcess.waitForFinished(5000)) {
		compileProcess.kill();
		throw SqlError(combineString("the compiling process maybe crashed where runId=", runId));
		return;
	}

	if(compileProcess.exitCode() || compileProcess.exitStatus() == Process::CrashExit) {
		throw CompileError(compileProcess.readAllStandardError());
		return;
	}
}

Compiler::~Compiler() {
#ifndef TEST
    std::string tmpDir = combineString(Config::getInstant()->getTmpUserPath(), runId);
    removeDir(tmpDir.c_str());
#endif
}

