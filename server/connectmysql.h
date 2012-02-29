/*
 * connectmysql.h 连接mysql数据库
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

#ifndef __CONNECTMYSQL_H
#define __CONNECTMYSQL_H

#include "../client/sqlError.h"
#include "config.h"
#include "mysqlManager.h"
#include <mysql.h>

extern MySQLManager db;

static bool connectMysql() {
    Config *config = Config::getInstant();
	db.setHostName(config->getDBHostName());
	db.setUserName(config->getDBUserName());
	db.setPassword(config->getDBPassWord());
	db.setDatabaseName(config->getDBDataBase());
    db.connect();

	if(!db.connected()) {
		throw SqlError(db.getLastError());
		return false;
	}

	return true;
}
#endif

