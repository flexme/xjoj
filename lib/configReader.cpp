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

#include "configReader.h"
#include "stringUtil.h"
#include <stdio.h>

ConfigReader::ConfigReader(std::string filename, ConfigReader::ConfigType type) {
    _impl = NULL;
    _configType = type;
    _status = OK;
    
    switch(_configType) {
        case SampleINI:
            _impl = new SampleINIConfigParser(filename);
            break;

        default:
            //config file type is not supported
            _status = BAD_TYPE;
    }

    _status = _impl->parse(dic);
}

ConfigReader::~ConfigReader() {
    if(_impl)
        delete _impl;
}

//parse a ini config file, store the key-value pairs in a map
ConfigReader::ConfigStatus SampleINIConfigParser::parse(std::map<std::string, ConfigValue>& dic) const {
    FILE *fp = fopen(filename.c_str(), "r");
    if(fp == NULL) {
        //file is not exits, or can not be accessed
        return ConfigReader::ACCESS_ERROR;
    }

    char buf[MAXLINE];
    char w1[MAXLINE];       //key
    char w2[MAXLINE];       //value

    while(fgets(buf, MAXLINE, fp)) {
        /*delete the last character '\n'*/
        char *ptr = buf;
        while(*ptr) {
            if(*ptr == '\n' || * ptr == '\r') {
                *ptr = 0;
                break;
            }
            ptr++;
        }

        trim(buf);
        if(buf[0] == '#' || buf[0] == '/' || buf[0] == 0) {
            //skip this line
            continue;
        }

        //file format:
        //key = value
        ptr = buf;
        int ind = 0;
        w1[0] = 0;
        w2[0] = 0;
        while(*ptr) {
            if(*ptr == '=') break;
            w1[ind++] = *ptr++;
        }
        w1[ind] = 0;
        if(*ptr != '=') return ConfigReader::BAD_FILE_FORMAT;
        trim(w1);
        if(w1[0] == 0) return ConfigReader::BAD_FILE_FORMAT;

        ptr++;
        ind = 0;
        while(*ptr) {
            w2[ind++] = *ptr++;
        }
        w2[ind] = 0;
        trim(w2);
        dic[std::string(w1)] = ConfigValue(w2);
    }

    
    fclose(fp);

    return ConfigReader::OK;
}

