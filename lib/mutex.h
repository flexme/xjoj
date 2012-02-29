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

#ifndef  MUTEX_H
#define  MUTEX_H

#include <pthread.h>

class Mutex {
public:
    explicit Mutex();
    ~Mutex();

    void lock();
    bool tryLock();
    bool tryLock(int timeout);
    void unlock();

private:
    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);
    const Mutex& operator=(const Mutex&) const;

    pthread_mutex_t mutex;
};

class MutexLocker {
public:
    inline explicit MutexLocker(Mutex *m) : mtx(m) {
        mtx->lock();
    }

    inline ~MutexLocker() {
        mtx->unlock();
    }

private:
    MutexLocker(const MutexLocker&);
    MutexLocker& operator=(const MutexLocker&);
    const MutexLocker& operator=(const MutexLocker&) const;

    Mutex *mtx;
};

#endif   /* ----- #ifndef MUTEX_H  ----- */

