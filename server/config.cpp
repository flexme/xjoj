/*
 * config.cpp   整个程序的配置类，单例模式
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

#include "config.h"
#include "configReader.h"
#include "log.h"
#include <unistd.h>

Config *Config::config = NULL;
const std::string Config::configFile = "etc/server_config.ini";

Config::Config() {
    listenPort = 1213;
    clientInfoPort = 1214;
    iniError = false;
    dataInPath = "data/in/";
    dataInPath = checkDir(dataInPath);
    dataOutPath = "data/out/";
    dataOutPath = checkDir(dataOutPath);
    spjPath = "data/spj/";
    spjPath = checkDir(spjPath);
    char buf[512];
    getcwd(buf, 512);
    logRoot = buf;
    logFileName = DEFAULT_LOGFILE;
    initConfig();
}

inline std::string Config::checkDir(std::string dir) {
    if(dir[dir.size() - 1] != DIR_SEPARTOR) {
        dir.push_back(DIR_SEPARTOR);
    }
    return dir;
}

void Config::initConfig() {
    if(access(configFile.c_str(), R_OK) == 0) {
        ConfigReader serverConfig(configFile, ConfigReader::SampleINI);
        int status = serverConfig.status();

        if(status == ConfigReader::OK) {
            dbHostName = serverConfig.getValue("dbHostName", dbHostName).toString();
            dbUserName = serverConfig.getValue("dbUserName", dbUserName).toString();
            dbPassword = serverConfig.getValue("dbPassword", dbPassword).toString();
            dbDatabase = serverConfig.getValue("dbDatabase", dbDatabase).toString();
            listenPort = serverConfig.getValue("listenPort", listenPort).toInt();
            clientInfoPort = serverConfig.getValue("clientInfoPort", clientInfoPort).toInt();
            dataInPath = serverConfig.getValue("dataInPath", dataInPath).toString();
            dataOutPath = serverConfig.getValue("dataOutPath", dataOutPath).toString();
            spjPath = serverConfig.getValue("spjPath", spjPath).toString();
            password = serverConfig.getValue("password", password).toString();
            logRoot = serverConfig.getValue("logRoot", logRoot).toString();
            logFileName = serverConfig.getValue("logFileName", logFileName).toString();

            if(access(dataInPath.c_str(), R_OK) != 0) {
                this->errMesg = std::string("data in path:") + dataInPath + " doesn't exists";
                iniError = true;
                return;
             }
             dataInPath = checkDir(dataInPath);

             if(access(dataOutPath.c_str(), R_OK) != 0) {
                this->errMesg = std::string("data out path:") + dataOutPath + " doesn't exists";
                iniError = true;
                return;
             }
             dataOutPath = checkDir(dataOutPath);

             if(access(spjPath.c_str(), R_OK) != 0) {
                this->errMesg = std::string("spj path:") + spjPath + " doesn't exists";
                iniError = true;
                return;
             }
             spjPath = checkDir(spjPath);
        } else  if(status == ConfigReader::ACCESS_ERROR) {
            iniError = true;
            this->errMesg = std::string("can't access the conf file ") + configFile;
            return;
        } else if(status == ConfigReader::BAD_FILE_FORMAT){
            iniError = true;
            this->errMesg = std::string("error format of ") + configFile;
            return;
        } else {
            this->errMesg = "unkonw error of " + configFile;
            return;
        }
    } 
}

