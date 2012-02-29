/*
 * defaultJudge.h
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

#ifndef __DEFAULTJUDGE_H
#define __DEFAULTJUDGE_H

#include "judge.h"
#include <string>

class TextFile {
public:
    int read();
    
    bool fail() {
        return this->fd < 0;
    }
    
    TextFile(const std::string& filename);

    ~TextFile();

    int skipWhiteSpaces();

private:
    int next();

private:
    TextFile(const TextFile&);
    TextFile& operator=(const TextFile&);
    const TextFile& operator=(const TextFile&) const;

    int fd;
    char buffer[1024];
    int bufferSize;
    char *p;
    std::string filename;
};

class DefaultJudge : public Judge {
public:
	DefaultJudge(const std::string& uo, const std::string& so) : Judge(uo), standardOut(so) {
    
    }
    
	JudgeResult judge();
    
private:
    std::string standardOut;
};

#endif

