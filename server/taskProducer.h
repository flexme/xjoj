/*
 * taskProducer.h
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

#ifndef  TASKPRODUCER_INC

#include <string>
#include <deque>
#include "thread.h"
#include "Semaphore.h"
#include "mutex.h"

#define MAX_TASK 10

struct Task {
    std::string runId;
    std::string proId;
    std::string time;
    std::string memory;
    std::string casenum;
    std::string spj;
    std::string langId;
};

extern std::deque<Task> tasks;
extern Semaphore full;
//extern Semaphore empty;
extern Mutex mutex;
extern Mutex sqlMutex;

class TaskProducer : public Thread {
public:
    TaskProducer();
    void run();
};

#define  TASKPRODUCER_INC

#endif   /* ----- #ifndef TASKPRODUCER_INC  ----- */

