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

#ifndef  THREAD_H
#define  THREAD_H

class ThreadData;

#include <pthread.h>
#include "mutex.h"

class Thread {
public:
    static pthread_t currentThreadId();
    static Thread *currentThread();

    explicit Thread();
    explicit Thread(ThreadData *d);
    virtual ~Thread();

    bool isFinished() const;
    bool isRunning() const;

    void setStackSize(unsigned int stackSize);
    unsigned int getStackSize() const;

    void join();
    void start();
    void terminate();

    static void* start(void *arg);
    static void finish(void *);

protected:
    virtual void run() = 0;

private:
    friend class ThreadData;

    bool running;
    bool finished;
    bool terminated;

    unsigned long stackSize;

    pthread_t thread_id;

    ThreadData *data;

    Mutex waitMutex; 

    Thread(const Thread&);
    Thread& operator=(const Thread&);
    const Thread& operator=(const Thread&) const;
};

class ThreadData {
public:
    ThreadData();
    ~ThreadData();

    static ThreadData *current();
    static ThreadData *get2(Thread *thread) {
        return thread->data;
    }

    Thread *thread;
    Mutex mutex;
};

#endif

