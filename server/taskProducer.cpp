/*
 * taskProducer.cpp 用来产生评测任务的生产者线程
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

#include "taskProducer.h"
#include "config.h"
#include "log.h"
#include "mysqlManager.h"
#include "connectmysql.h"
#include "stringUtil.h"
#include <unistd.h>
#include <stdlib.h>

std::deque<Task> tasks;

Semaphore full(0);
//Semaphore empty(MAX_TASK);
Mutex mutex;
Mutex sqlMutex;

TaskProducer::TaskProducer() {
    
}

void TaskProducer::run() {
    while(1) {
        //empty.acquire();
        mutex.lock();

        if(tasks.size() < MAX_TASK) {
            int r = MAX_TASK - tasks.size();
            std::string sql = combineString("select runId,problemId,languageId,authorId from status where poolStatus=0 and judgeStatus='Waiting' order by runId asc limit 0,", r);

            sqlMutex.lock();
            db.executeResult(sql);
            std::vector<std::vector<std::string> > result = db.getResult();

            for(int i = 0; i < result.size(); i++) {
                Task tmptask;
                tmptask.runId = result[i][0];
                tmptask.proId = result[i][1];
                tmptask.langId = result[i][2];

                sql = "select timelimit,memorylimit,spj from problem where problemId=" + tmptask.proId;
                db.executeResult(sql);
                std::vector<std::vector<std::string> > tq = db.getResult();
                tmptask.time = tq[0][0];
                tmptask.memory = tq[0][1];
                tmptask.spj = tq[0][2];

                int pid = atoi(tmptask.proId.c_str());
                int cnum = 0;
                while(1) {
                    if(access(Config::getInstant()->getDataIn(pid, cnum + 1).c_str(), R_OK) == 0) {
                        cnum++;
                    }else
                        break;
                }
                tmptask.casenum = combineString(cnum);

                tasks.push_back(tmptask);

                db.execute("update status set poolStatus=1 where runId=" + tmptask.runId);
                full.release();
            }
            sqlMutex.unlock();
        }
        mutex.unlock();
        sleep(1);
    }
}

