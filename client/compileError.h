/*
 * compilerError.h 声明compilerError类，在编译过程中产生的错误
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

#ifndef __COMPILER_ERROR_H
#define __COMPILER_ERROR_H

#include "error.h"

class CompileError : public Error {
public:
    std::string toString() const {
		return errInfo;
	}

	CompileError(std::string ei) : errInfo(ei) {
    
    }

private:
    CompileError operator=(const CompileError&);
    const CompileError operator=(const CompileError&) const;

private:
    std::string errInfo;
};

#endif

