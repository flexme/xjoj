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

#ifndef __PROTOCAL_H
#define __PROTOCAL_H

//runId problemId tle mle casenum spj langId
//runId : int
//tle			 : int
//mle			 : int
//spj			 : bool
//langId         : int
//
#define GET_TASK 9001
#define REPLY_TASK 9002
#define UPDATE_STATUS 9003
#define UPDATE_RUNINFO 9004
#define UPDATE_CASE 9006
#define OK 9005
#define AUTH 9007
#define UPDATE_COMERROR 9008
#define GET_COMPILERINFO 9009
#define GET_SOURCECODE 9010
#define DATA_SYNC 9011
#define GET_DATA 9012
#endif

