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

#ifndef  FILE_H
#define  FILE_H

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define DIR_SEPARTOR '/'

ssize_t readn(int fd, const void *buffer, size_t count);

ssize_t nonBlockReadn(int fd, const void *buffer, size_t count);

ssize_t writen(int fd, const void *buffer, size_t count);

ssize_t nonBlockWriten(int fd, const void *buffer, size_t count);

int copyFile(const char* src, const char* dest);

int removeDir(const char* dir, bool delRoot = true);

unsigned int fileSize(const char *filename);

//create a pipe, and auto set FD_CLOEXEC flag on it
//so when forking a child process and call the exec system call, the pipe will automatic closed
static inline int safe_pipe(int pipefd[2], int flags = 0) {
    register int ret;

    ret = pipe(pipefd);
    if(ret == -1)
        return -1;

    fcntl(pipefd[0], F_SETFD, FD_CLOEXEC);
    fcntl(pipefd[1], F_SETFD, FD_CLOEXEC);

    if(flags & O_NONBLOCK) {
        fcntl(pipefd[0], F_SETFL, fcntl(pipefd[0], F_GETFL) | O_NONBLOCK);
        fcntl(pipefd[1], F_SETFL, fcntl(pipefd[1], F_GETFL) | O_NONBLOCK);
    }

    return 0;
}
#endif   /* ----- #ifndef FILE_H  ----- */

