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

#include "mysqlManager.h"
#include <mysql.h>
#include <stdio.h>

void MySQLManager::connect() {
    if(isConnected) {
        freeResult();
        close();
    }

    mysql_init(&con);

    if(!mysql_real_connect(&con, host.c_str(), userName.c_str(), password.c_str(), dbName.c_str(), port, NULL, 0)) {
        lastError = mysql_error(&con);
        return;
    }

    isConnected = true;
}

void MySQLManager::close() {
    freeResult();
    mysql_close(&con);
    isConnected = false;
}

bool MySQLManager::execute(std::string sql) {
    if(!isConnected)
        return false;

    if(sql.size() == 0)
        return false;

    int ret = mysql_real_query(&con, sql.c_str(), sql.size());
    if(ret) {
        lastError = mysql_error(&con);
        return false;
    }

    return true;
}

bool MySQLManager::executeResult(std::string sql) {
    if(!isConnected)
        return false;

    if(sql.size() == 0)
        return false;

    freeResult();

    int ret = mysql_real_query(&con, sql.c_str(), sql.size());
    if(ret) {
        lastError = mysql_error(&con);
        return false;
    }

    MYSQL_RES *res;
    MYSQL_ROW row;

    res = mysql_store_result(&con);
    std::vector<std::string> objectValue;
    int fn = mysql_num_fields(res);
    while(row = mysql_fetch_row(res)) {
        objectValue.clear();
        for(int j = 0; j < fn; j++) {
            objectValue.push_back(row[j]);
        }

        resultList.push_back(objectValue);
    }

    mysql_free_result(res);

    return true;
}

