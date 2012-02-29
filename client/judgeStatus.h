/*
 * judgeStatus.h 定义评判中的各种状态的数值
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

#ifndef __JUDGE_STATUS_H
#define __JUDGE_STATUS_H

#define COMPILING 					    1001		//Compiling
#define RUNNING 						1002		//Running
#define RUNTIME_ERROR				    1003		//Runtime Error
#define ACCEPTED						1004		//Accepted
#define WRONG_ANSWER					1005		//Wrong Answer
#define TIME_LIMIT_EXCEEDED		        1006		//Time Limit Exceeded
#define MEMORY_LIMIT_EXCEEDED		    1007		//Memory Limit Exceeded
#define OUTPUT_LIMIT_EXCEEDED 	        1008		//Output Limit Exceeded
#define COMPILATION_ERROR			    1009		//Compliation Error
#define PRESENTATION_ERROR 		        1010		//Presentation Error
#define FLOATING_POINT_ERROR		    1011		//Floating Point Error
#define SEGMENTATION_FAULT			    1012 		//Segmentation Fault
#define READY							1013		//Ready
#define SERVER_ERROR 				    1014		//Server Error
#define RESTRICTED_FUNCTION             1015        //RESTRICTED_FUNCTION
#define JUDGING                         1016        //Judging

typedef int JudgeResult;

#endif

