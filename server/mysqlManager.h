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


#ifndef  MYSQLMANAGER_INC
#define  MYSQLMANAGER_INC

#include <string>
#include <vector>
#include <mysql.h>

class MySQLManager {
public:
    MySQLManager(std::string host, std::string userName, std::string password, std::string dbName, unsigned int port = DEFAULT_PORT) :
        isConnected(false),
        host(host),
        userName(userName),
        password(password),
        dbName(dbName),
        port(port) {
        
        }

    MySQLManager() {
        port = DEFAULT_PORT;
    }

    ~MySQLManager() {
        close();
    }

    void setHostName(std::string host) {
        this->host = host;
    }

    void setUserName(std::string userName) {
        this->userName = userName;
    }

    void setPassword(std::string password) {
        this->password = password;
    }

    void setPort(unsigned int port) {
        this->port = port;
    }

    void setDatabaseName(std::string dbName) {
        this->dbName = dbName;
    }

    const std::string& getLastError() const {
        return lastError;
    }

    void connect();

    void close();

    bool connected() const {
        return isConnected;
    }

    std::vector< std::vector<std::string> > getResult() {
        return resultList;
    }
    
    void freeResult() {
        for(int i = 0; i < resultList.size(); i++)
            resultList[i].clear();

        resultList.clear();
    }

    bool execute(std::string sql);

    bool executeResult(std::string sql);

    static const unsigned int DEFAULT_PORT = 3306;

private:
    bool isConnected;
    
    std::vector< std::vector<std::string> > resultList;

    MYSQL con;

    std::string lastError;

    unsigned int port;

    std::string host;

    std::string userName;

    std::string password;

    std::string dbName;
};
#endif   /* ----- #ifndef MYSQLMANAGER_INC  ----- */

