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

#ifndef __SPECIAL_JUDGE_H
#define __SPECIAL_JUDGE_H

#include "judge.h"

class SpecialJudge : public Judge {
public:
	SpecialJudge(const std::string& uo, const std::string& stdInFile, 
                const std::string& stdOutFile, const std::string& spjfile):
        Judge(uo),
        stdInFile(stdInFile),
        stdOutFile(stdOutFile),
        judgeFileName(spjfile) {

        }

	JudgeResult judge();

private:
    std::string judgeFileName;
    std::string stdInFile;
    std::string stdOutFile;
};

#endif

