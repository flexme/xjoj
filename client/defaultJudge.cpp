/*
 * defaultJudge.cpp
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

#include "defaultJudge.h"
#include "judgeStatus.h"
#include "sqlError.h"
#include "util.h"
#include "log.h"
#include <unistd.h>
#include <fcntl.h>

TextFile::TextFile(const std::string& filename):
				fd(0),
				bufferSize(0),
				filename(filename) {
                    this->p = this->buffer;
                    this->fd = open(filename.c_str(), O_RDONLY);
                    if(this->fd < 0) {
                        BLOG("TextFile::TextFile(): Fail to open " + filename, SYSCALL_ERROR);
                    }
                    this->bufferSize = sizeof(this->buffer);
                    this->p = this->buffer + this->bufferSize;
                }

TextFile::~TextFile() {
    if(this->fd > 0) {
		close(this->fd);
	}
}

int TextFile::next() {
    if(this->p - this->buffer >= this->bufferSize) {
        if(this->bufferSize < sizeof(this->buffer)) {
            return 0;
        }
        this->bufferSize = readn(this->fd, this->buffer, sizeof(this->buffer));
        if(int(this->bufferSize) < 0) {
            BLOG("TextFile::next(): Fail to read from " + filename, SYSCALL_ERROR);
            return -1;
        }
        if((int)this->bufferSize == 0) {
            return 0; 
        }
        this->p = this->buffer;
    }    
    return *this->p;
}

int TextFile::read() {
    if(this->fd < 0) {
        return -1;
    }
    int ret = this->next();
    if(ret > 0) {
        this->p++;
    }
    if(ret != '\r') {
        return ret;
    }

    int t = next();
    if(t == '\n') {
        this->p++;
    }
    return '\n';
}

int TextFile::skipWhiteSpaces() {
	int ret;
	do { ret = this->read(); } while(ret > 0 && isspace(ret) && ret != '\n');
	return ret;
}

JudgeResult DefaultJudge::judge(){
	int ret = ACCEPTED;
    bool canPE = false;
    bool PEEQ = true;

	TextFile f1(standardOut), f2(userOut);

	if(f1.fail() || f2.fail()) {
		return SERVER_ERROR;
	}

	int c1 = f1.skipWhiteSpaces(), c2 = f2.skipWhiteSpaces();

	while(c1 > 0 && c2 > 0) {
        if(canPE && !isspace(c1) && !isspace(c2)) {
            if(!PEEQ) {
                return WRONG_ANSWER;
            }
            ret = PRESENTATION_ERROR;
        }
        PEEQ = true;
		if(c1 == c2) {
			c1 = f1.read();
			c2 = f2.read();
		}else if(!isspace(c1) && !isspace(c2)) {
			return WRONG_ANSWER;
		} else {
            canPE = true;
            if(isspace(c1) && isspace(c2) && c1 != c2) {
                PEEQ = false;
            }
			if(isspace(c1)) c1 = f1.skipWhiteSpaces();
			if(isspace(c2)) c2 = f2.skipWhiteSpaces();
			//ret = PRESENTATION_ERROR;
		}
	}

	if(c1 < 0 || c2 < 0)
		return WRONG_ANSWER;

	if(c1 > 0 || c2 > 0) {
		if(c1>0) {
			if(isspace(c1)) {
				c1 = f1.skipWhiteSpaces();
			}
			if(c1 > 0 && c1 != (int)'\n')
				return WRONG_ANSWER;
		} else {
			if(isspace(c2)) {
				c2 = f2.skipWhiteSpaces();
			}
			if(c2 > 0 && c2 != (int)'\n') 
				return WRONG_ANSWER;
		}
		if(c1 < 0 || c2 < 0)
			return WRONG_ANSWER;
		//return PRESENTATION_ERROR;
	}
	return ret;
}

