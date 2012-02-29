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

#include "stringUtil.h"
#include <stdlib.h>
#include <cstring>

void trim(char* str) {
    char *start;
    char *end;
    start = str;
    while(*start == ' ') {
        start++;
    }
    end = start;
    while(*end) end++;
    end--;
    while(*end == ' ') end--;
    char* index = str;
    while(start <= end) {
        *index++ = *start++;
    }
    *index = 0;
}

std::string trim(const std::string& str) {
    int len = str.size() + 1;
    char *buf = (char*)malloc(len);
    memcpy(buf, str.c_str(), len);
    trim(buf);
    std::string ret(buf);
    free(buf);
    return ret;
}

std::string formatTime(const char* format) {
    char buf[1024];
    time_t t;
    tm tt;
    t = time(NULL);
    //use localtime_r in multithread evironment
    localtime_r(&t, &tt);
    strftime(buf, 1024, format, &tt);
    return std::string(buf);
}

std::string& replace(std::string& str, std::string need, std::string rep) {
    size_t ind = str.find(need);
    if(ind == std::string::npos)
        return str;

    return str.replace(ind, need.size(), rep);
}

std::string& replaceAll(std::string& str, std::string need, std::string rep) {
    size_t ind = 0;
    while(1) {
        ind = str.find(need, ind);
        if(ind == std::string::npos)
            break;

        str.replace(ind, need.size(), rep);
        ind += rep.size();
    }

    return str;
}

