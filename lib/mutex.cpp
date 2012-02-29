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

#include "mutex.h"
#include <pthread.h>

Mutex::Mutex() {
    pthread_mutex_init(&mutex, NULL);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex);
}

void Mutex::lock() {
    pthread_mutex_lock(&mutex);
}

bool Mutex::tryLock() {
    return pthread_mutex_trylock(&mutex) == 0;
}

void Mutex::unlock() {
    pthread_mutex_unlock(&mutex);
}

