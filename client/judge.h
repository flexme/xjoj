/*
 * judge.h
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

#ifndef __JUDGE_H
#define __JUDGE_H

#include "judgeStatus.h"
#include <string>

class Judge {
public:
	Judge(const std::string& uo) : userOut(uo) {
    
    }
    
	virtual JudgeResult judge() = 0;

	virtual ~Judge() {
		//if(userOut.length() > 0)
			//QFile::remove(userOut);
	}

protected:
    std::string userOut;
};

#endif

