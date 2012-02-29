/*
 * log.cpp
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

#include "log.h"
#include "stringUtil.h"
#include <stdio.h>

const char* Log::LEVEL_NAME[5] = {"DEBUG", "ERROR", "WARN", "INFO", "SYSCALL_ERROR"};
Log* Log::log_ = NULL;

void DiskLogFile::createNewLogFile() {
    if(file != NULL) {
        fclose(file);
        file = NULL;
    }

    file = fopen(logFileName.c_str(), "a+");
    if(file == NULL) {
        std::string err_mesg = strerror(errno);
        openlog("XJOJ Judge Client", 0 ,LOG_USER);
        syslog(LOG_ERR, "Fail to create file %s : %s", logFileName.c_str(), err_mesg.c_str());
    }
}

void DiskLogFile::write(const std::string& message) {
    if(file == NULL) {
        createNewLogFile();
    } 

    if(file) {
        fputs(message.c_str(), file);
        fflush(file);
    }
}

void DiskLogFile::close() {
    if(file) {
        fclose(file);
        file = NULL;
    }
}

DiskLogFile::~DiskLogFile() {
    close();
}

Log::Log(const std::string& logFileName) {
    logFile = new DiskLogFile(logFileName);
}

Log* Log::getInstance() {
    if(log_ == NULL) {
        log_ = new Log(DEFAULT_LOGFILE);
    }
    return log_;
}

Log* Log::getInstance(const std::string& logFileName) {
    if(log_ == NULL) {
        log_ = new Log(logFileName);
    }
    return log_;
}

void Log::log(const std::string& message, int level, bool showConsole) {
    if(showConsole || logFile) {
        std::string wm = formatTime("%m-%d %H:%M:%S ");
        wm.append(combineString('[', LEVEL_NAME[level], "] ", message));
        if(level == SYSCALL_ERROR) {
            wm.append(" ");
            wm.append(strerror(errno));
        }
        wm.push_back('\n');

        //lockf(1, 1, 0);
        if(showConsole) {
            printf("%s", wm.c_str());
        }

        if(logFile) {
            logFile->write(wm);
        }
        //lockf(1, 0, 0);
    }    
}

void Log::log(const std::string& filename, int lineNumber, std::string message, int level, bool showConsole) {
    if(showConsole || logFile) {
        std::string wm = formatTime("%m-%d %H:%M:%S ");
        wm.append(combineString('[', LEVEL_NAME[level], " ]", filename));
        wm.append(combineString(": ", lineNumber, ": ", message));

        if(level == SYSCALL_ERROR) {
            wm.append(strerror(errno));
        }
        wm.push_back('\n');

        //lockf(1, 1, 0);
        if(showConsole) {
            printf("%s", wm.c_str());
        }

        if(logFile) {
            logFile->write(wm);
        }
        //lockf(1, 0 ,0);
    }
}

