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

#ifndef  STRINGUTIL_H
#define  STRINGUTIL_H

#include <string>
#include <sstream>
#include <time.h>

void trim(char* str);

std::string trim(const std::string& str);

std::string& replace(std::string& str, std::string need, std::string rep);

std::string& replaceAll(std::string& str, std::string need, std::string rep);

template<class T>
std::string combineString(T v) {
    std::stringstream sin;
    sin << v;
    return sin.str();
}

template<class T1, class T2>
std::string combineString(T1 v1, T2 v2) {
    std::stringstream sin;
    sin << v1 << v2;
    return sin.str();
}

template<class T1, class T2, class T3>
std::string combineString(T1 v1, T2 v2, T3 v3) {
    std::stringstream sin;
    sin << v1 << v2 << v3;
    return sin.str();
}

template<class T1, class T2, class T3, class T4>
std::string combineString(T1 v1, T2 v2, T3 v3, T4 v4) {
    std::stringstream sin;
    sin << v1 << v2 << v3 << v4;
    return sin.str();
}

std::string formatTime(const char* format);

#endif   /* ----- #ifndef STRINGUTIL_H  ----- */

