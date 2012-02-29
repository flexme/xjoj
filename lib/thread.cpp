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

#include "thread.h"
#include "log.h"

static pthread_once_t current_thread_data_once = PTHREAD_ONCE_INIT;
static pthread_key_t current_thread_data_key;

static void destroy_current_thread_data(void *p) {
    pthread_setspecific(current_thread_data_key, p);
    delete reinterpret_cast<ThreadData *>(p);
    pthread_setspecific(current_thread_data_key, 0);
}

static void create_current_thread_data_key() {
    pthread_key_create(&current_thread_data_key, destroy_current_thread_data);
}

ThreadData *ThreadData::current() {
    pthread_once(&current_thread_data_once, create_current_thread_data_key);

    ThreadData *data = reinterpret_cast<ThreadData *>(pthread_getspecific(current_thread_data_key));
    if(!data) {
        //
    }

    return data;
}

ThreadData::ThreadData() : thread(0) {

}

ThreadData::~ThreadData() {
    //delete it's thread
    Thread *t = thread;
    thread = 0;
    delete t;
}

Thread::Thread(ThreadData *d) : running(false), finished(false), terminated(false), stackSize(0), data(d) {
    thread_id = 0;
    if(!data) {
        data = new ThreadData();
    }
}

Thread::Thread() : running(false), finished(false), terminated(false), stackSize(0), data(new ThreadData()) {
    thread_id = 0;
    data->thread = this;
}

Thread::~Thread() {
    //actually the data is already deleted
    MutexLocker locker(&data->mutex);
    if(running && !finished) {
        BLOG("Thread::destroy while thread is still running", ERROR);
    }

    waitMutex.unlock();

    DLOG(INFO) << "thread is destroyed\n";
    data->thread = 0;
}

bool Thread::isFinished() const {
    MutexLocker locker(&data->mutex);
    return finished;
}

bool Thread::isRunning() const {
    MutexLocker locker(&data->mutex);
    return running;
}

void Thread::setStackSize(unsigned int stackSize) {
    MutexLocker locker(&data->mutex);
    if(running)
        return;

    this->stackSize = stackSize;
}

unsigned int Thread::getStackSize() const {
    return this->stackSize;
}

Thread *Thread::currentThread() {
    ThreadData *data = ThreadData::current();
    return data->thread;
}

void Thread::start() {
    MutexLocker locker(&data->mutex);

    if(running) 
        return;

    running = true;
    finished = false;
    terminated = false;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if(stackSize > 0) {
        int code = pthread_attr_setstacksize(&attr, stackSize);
        if(code) {
            running = false;
            finished = false;
            return;
        }
    }

    int code = pthread_create(&thread_id, &attr, Thread::start, this);
    pthread_attr_destroy(&attr);

    if(code) {
        running = false;
        finished = false;
        thread_id = 0;
    }

    waitMutex.lock();
}

void Thread::terminate() {
    MutexLocker locker(&data->mutex);

    if(!thread_id) 
        return;

    int code = pthread_cancel(thread_id);
    if(code) {
        BLOG("Thread::Thread termination error", ERROR);
    } else {
        terminated = true;
    }
}


pthread_t Thread::currentThreadId() {
    return pthread_self();
}

void *Thread::start(void *arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_cleanup_push(Thread::finish, arg);
    //
    Thread *thr = reinterpret_cast<Thread *>(arg);
    ThreadData *data = ThreadData::get2(thr);

    pthread_once(&current_thread_data_once, create_current_thread_data_key);
    pthread_setspecific(current_thread_data_key, data);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();

    thr->run();

    pthread_cleanup_pop(1);

    return 0;
}

void Thread::finish(void *arg) {
    Thread *thr = reinterpret_cast<Thread *>(arg);

    thr->data->mutex.lock();

    thr->running = false;
    thr->finished = true;
    thr->terminated = false;
    thr->thread_id = 0;

    thr->data->mutex.unlock();
}

void Thread::join() {
    if(!thread_id)
        return;

    if(thread_id == pthread_self())
        return;

    //the waitMutex is locked when this thread is created
    //so if the thread is not exited, the calling thread will bolck
    waitMutex.lock();
    waitMutex.unlock();

    //pthread_join(thread_id, NULL);
}

