/*
 * config.h
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

#ifndef __CONFIG_H
#define __CONFIG_H

#include <map>
#include <string>
#include "configReader.h"
#include "stringUtil.h"
#include "file.h"

void printUsage();

class Config {
private:
    static Config *config;
    static const std::string configFile;

    int job_uid;

    int job_gid;

    std::string dataInPath;

    std::string dataOutPath;

    std::string tmpSourcePath;

    std::string tmpUserPath;
   
    std::string spjPath;

    int threadNum;

    std::pair< std::string, int> serverAddress;

    std::string errMesg;

    bool iniError;

    std::string logRoot;

    std::string logFileName;

    std::string password;

    std::string rootPath;

    unsigned long myip;

    bool needto_sync_data;

private:

    bool initConfig();

    std::string checkDir(std::string dir);

    Config();

    Config(const Config&);

    Config& operator=(const Config&);
    const Config& operator=(const Config&) const;

public:
    static Config *getInstant() {
        if(config == NULL) {
            config = new Config();
        }
        return config;
    }

    bool needToSyncData() const {
        return needto_sync_data;
    }

    int getJobUID() const {
        return this->job_uid;
    }

    int getJobGID() const {
        return this->job_gid;
    }

    std::string getTmpSourcePath() const {
        return this->tmpSourcePath;
    }

    std::string getTmpUserPath() const {
        return this->tmpUserPath;
    }

    std::string getSpjPath() const {
        return this->spjPath;
    }

    std::pair< std::string, int> getServer() const {
        return this->serverAddress;
    }

    bool initConfig(int argc, char *argv[]);

    std::string getErrorMesg() const {
        return errMesg;
    }

    bool isIniError() const {
        return iniError;
    }

    std::string getLogRoot() const {
        return this->logRoot;
    }

    std::string getLogFileName() const {
        return this->logFileName;
    }

    std::string getDataInPath() const {
        return dataInPath;
    }


    std::string getDataOutPath() const {
        return dataOutPath;
    }

    std::string getDataIn(int proId, int caseId) {
        return combineString(dataInPath, proId, DIR_SEPARTOR, caseId);
    }

    std::string getDataOut(int proId, int caseId) {
        return combineString(dataOutPath, proId, DIR_SEPARTOR, caseId);
    }

    std::string getUserOut(int runId) {
        return combineString(tmpUserPath, runId, DIR_SEPARTOR, "output");
    }

    std::string getSpjFile(int proId) {
        return combineString(spjPath, proId);
    }

    int getThreadNumber() const {
        return this->threadNum;
    }

    std::string getPassword() const {
        return this->password;
    }

    std::string getRootPath() const {
        return this->rootPath;
    }


    unsigned long getMyIp() const {
        return this->myip;
    }
};

#endif


