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

#ifndef  CONFIG_INC
#define  CONFIG_INC

#include <string>
#include "stringUtil.h"
#include "file.h"

class Config {
private:
    static Config* config;
    static const std::string configFile;

    std::string dbHostName;
    
    std::string dbUserName;
    
    std::string dbPassword;
    
    std::string dbDatabase;

    std::string dataInPath;

    std::string dataOutPath;

    std::string spjPath;

    std::string password;

    int listenPort;

    int clientInfoPort;
    
    std::string errMesg;

    bool iniError;

    std::string logRoot;

    std::string logFileName;

    void initConfig();

    std::string checkDir(std::string dir);

private:
    Config();

    Config(const Config&);

    Config& operator=(const Config&);
    const Config& operator=(const Config&) const;

public:
    static Config* getInstant() {
        if(config == NULL) {
            config = new Config();
        }
        return config;
    }

    std::string getDBHostName() const {
        return this->dbHostName;
    }

    std::string getDBUserName() const {
        return this->dbUserName;
    }

    std::string getDBPassWord() const {
        return this->dbPassword;
    }
    
    std::string getDBDataBase() const {
        return this->dbDatabase;
    }

    int getListenPort() const {
        return this->listenPort;
    }

    int getClientInfoPort() const {
        return this->clientInfoPort;
    }

    std::string getErrorMesg() const {
        return errMesg;
    }

    bool isIniError() const {
        return iniError; 
    }

    std::string getDataIn(int proId, int caseId) {
        return combineString(dataInPath, proId, DIR_SEPARTOR, caseId);
    }

    std::string getDataOut(int proId, int caseId) {
        return combineString(dataOutPath, proId, DIR_SEPARTOR, caseId);
    }

    std::string getSpjFile(int proId) {
        return combineString(spjPath, proId);
    }

    std::string getPassword() const {
        return this->password;
    }

    std::string getLogRoot() const {
        return this->logRoot;
    }

    std::string getLogFileName() const {
        return this->logFileName;
    }
};
#endif   /* ----- #ifndef CONFIG_INC  ----- */

