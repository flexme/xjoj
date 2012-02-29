/*
 * util.h 一些常用函数的实现
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

#ifndef __UTIL_H
#define __UTIL_H

#include "file.h"
#include <sstream>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <errno.h>

sighandler_t installSignalHandler(int signal,sighandler_t handler,int flags=0);

sighandler_t installSignalHandler(int signal,sighandler_t handler,int flags,sigset_t mask);

static inline int ReadUint32(int fd, unsigned int* value) {
    if (readn(fd, value, sizeof(*value)) < (int) sizeof(*value)) {
        return -1;
    }
    *value = ntohl(*value);
    return 0;
}

int CreateServerSocket(int*);

int write_pid(const char* pid_filename);

unsigned int get_myip();

int setLimit(int res, unsigned int limit);

void daemon_init();

#endif

