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

#ifndef  CONFIGREADER_H
#define  CONFIGREADER_H

#include <map>
#include <string>
#include <sstream>
#include <stdlib.h>

const int MAXLINE = 512;

class ConfigValue {
private:
    std::string str;

public:
    ConfigValue(std::string s) : str(s) {
    
    }

    ConfigValue() {
    
    }

    std::string toString() const {
        return str;
    }

    int toInt() const {
        return atoi(str.c_str());
    }
};

class ConfigParser;

class ConfigReader {
public:
    enum ConfigType {
        SampleINI
    };

    enum ConfigStatus {
        OK,                     //parse config file successfully
        ACCESS_ERROR,           //the config file does not exists, or can not be accessed
        BAD_FILE_FORMAT,        //the format of the config file is error 
        BAD_TYPE                //the config file type is not supported
    };

private:
    ConfigType _configType;
    ConfigStatus _status;
    ConfigParser *_impl;
    std::map<std::string, ConfigValue> dic;     //key-value pair

public:
    ConfigReader(std::string filename, ConfigType type);
    ~ConfigReader();

    ConfigReader::ConfigStatus status() const {
        return _status;
    }

    ConfigType configType() const {
        return _configType;
    }

    template<class T>
    ConfigValue getValue(std::string item, T def) {
        if(dic.count(item) == 0) {
            std::stringstream sin;
            sin << def;
            return ConfigValue(sin.str());
        }

        return dic[item];
    }
};

class ConfigParser {
protected:
    std::string filename;

public:
    ConfigParser(const std::string& fn) : filename(fn) {
    
    }

    virtual ~ConfigParser() {
    
    }
    
    virtual ConfigReader::ConfigStatus parse(std::map<std::string, ConfigValue>& con) const = 0;
};

class SampleINIConfigParser : public ConfigParser {
public:
    SampleINIConfigParser(const std::string& fn) : ConfigParser(fn) {
    
    }

    ConfigReader::ConfigStatus parse(std::map<std::string, ConfigValue> &con) const;
};

#endif   /* ----- #ifndef CONFIGREADER_H  ----- */

