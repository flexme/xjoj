/*
 * log.h
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

#ifndef __LOG_H
#define __LOG_H

#include <string>
#include <sstream>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>

#define DEBUG 0
#define ERROR 1
#define WARNING 2
#define INFO 3
#define SYSCALL_ERROR 4

#define LOG(level) BufLog()

#define DEFAULT_LOGFILE "log.txt"

//#define __DEBUG

#ifdef __DEBUG
#define DLOG(level) BufLog() 
#else
#define DLOG(level) if(0) BufLog()
#endif

// output the message to console
#define CLOG(message, level) {\
    std::string tt = formatTime("%m-%d %H:%M:%S"); \
    BufLog() << tt << " [" << Log::LEVEL_NAME[(level)]<< "] " << message << '\n';\
}

#define FLOG(message, level) Log::getInstance()->log(__FILE__, __LINE__, (message), (level))

#define BLOG(message, level) Log::getInstance()->log((message), (level));

#define LLOG(message, level) Log::getInstance()->log((message), (level), false);

class BufLog {
private:
    mutable std::stringstream sin;

public:
    template <class T>
    const BufLog& operator<<(T val) const {
        sin << val;
        return *this;
    }

    ~BufLog() {
        printf("%s", sin.str().c_str());
    }
};

class LogFile {
public:
    virtual ~LogFile() {
    
    }
    
    virtual void write(const std::string& message) = 0;

    virtual void close() = 0;
};

class DiskLogFile : public LogFile {
public:

    DiskLogFile(const std::string& logFileName) {
        this->logFileName = logFileName;
        file = NULL;
    }

    ~DiskLogFile();
    
    void write(const std::string& message);

    void close();

private:
    void createNewLogFile();

    std::string logFileName;
    FILE* file;
};

class Log {
private:
    static Log* log_;
    
    Log();

    Log(const std::string& logFileName);

public:
    static const char* LEVEL_NAME[5]; 

    static Log *getInstance();
    static Log *getInstance(const std::string& logFileName);

    void log(const std::string& message, int level, bool showConsole = true);

    void log(const std::string& filename, int lineNumber, std::string message, int level, bool showConsole = true);

    void close() {
        if(logFile) {
            logFile->close();
        }
    }

    ~Log() {
        close();
    }

private:
	Log(const Log&);

	Log& operator=(const Log&);
    const Log& operator=(const Log&) const;

    LogFile *logFile;

};

#endif

