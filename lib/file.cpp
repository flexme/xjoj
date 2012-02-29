/*
 *  Copyright (C) <2008>  <chenkun cs.kunchen@gmail.com>

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "file.h"
#include "log.h"
#include "stringUtil.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string>

//reading characters from fd(block file descriptor) and store them in buffer
//parameter count is the most bytes to be read
//return value:
//  if fd is closed or can not be read, -1 will be returned. otherwise the number of bytes
//  read is returned(zero indicates end of file).
//
ssize_t readn(int fd, const void *buffer, size_t count) {
	char *p = (char*)buffer;
	while(count > 0) {
		ssize_t num = read(fd, p, count);
		if(num == -1) {
			if(errno == EINTR) 
                continue;

			return -1;
		}
		if(num == 0) 
            break;

		p += num;
		count -= num;
	}
	return p - (char*)buffer;
}

//reading characters from fd(nonblock file descriptor) and store them in buffer
//parameter count is the most bytes to be read
//return value:
//  if fd is closed or can not be read, -1 will be returned. otherwise the number of bytes
//  read is returned(zero indicates no data is avaliable).
//
ssize_t nonBlockReadn(int fd, const void* buffer, size_t count) {
    char *p = (char*)buffer;
    while(count > 0) {
        ssize_t num = read(fd, p, count);
        if (num == -1) {
            if (errno == EINTR) {
                // interrupted by a signals, read again
                continue;
            }

            if (errno == EWOULDBLOCK || errno == EAGAIN)
                //no data to read
                break;

            return -1;
        }

        if (num == 0) {
            break;
        }

        p += num;
        count -= num;
    }

    return p - (char*)buffer;
}

//write data to fd(block file descriptor)
//return value:
//  if fd is closed or can not be writen, -1 will be returned. otherwise the number of bytes
//  writen is returned.
//
ssize_t writen(int fd, const void *buffer, size_t count) {
    char *p = (char*)buffer;
    while(count > 0) {
        ssize_t num = write(fd, p, count);
        if(num == -1) {
            if(errno == EINTR) 
                continue;

            return -1;
        }
        if(num == 0) 
            break;

        p += num;
        count -= num;
    }
    return p - (char*)buffer;
}

//write data to fd(nonblock file descriptor)
//return value:
//  if fd is closed or can not be writen, -1 will be returned. otherwise the number of bytes
//  writen is returned.
//
ssize_t nonBlockWriten(int fd, const void *buffer, size_t count) {
    char *p = (char*)buffer;
    while(count > 0) {
        ssize_t num = write(fd, p, count);
        if(num == -1) {
            if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
                //continue to write
                continue;

            return -1;
        }

        if(num == 0)
            break;

        p += num;
        count -= num;
    }

    return p - (char*)buffer;
}

//copy a file from src to dest
//return value:
//  if source file is not exists, or can not be read, -1 is returned;
//  if destination file is already exists, or can not create a new file, -1 is returned
//  if the copying process occur some error, -1 is returned.
//  otherwise 0 is returned.
//
int copyFile(const char *src, const char *dest) {
    if(access(src, R_OK) < 0)
        return -1;

    if(access(dest, R_OK) == 0)
        return -1;

    int fd_read = open(src, O_RDONLY);
    if(fd_read < 0)
        return -1;

    int fd_write = open(dest, O_WRONLY | O_CREAT, 0777);
    if(fd_write == -1) {
        close(fd_read);
        return -1;
    }

    char buf[4096];
    while(1) {
        int nread = readn(fd_read, buf, sizeof(buf));
        if(nread < 0) {
            close(fd_read);
            close(fd_write);
            return -1;
        }

        if(nread == 0)
            break;

        int nwrite = writen(fd_write, buf, nread);
        if(nwrite != nread) {
            close(fd_read);
            close(fd_write);
            return -1;
        }
    }

    close(fd_read);
    close(fd_write);
    return 0;
}

//delete a dir(recursive)
//return value:
//  if dir is not exists, -1 is returned
//  if some error occurs, -1 is returned(errno is set)
//  otherwise 0 is returned
//
int removeDir(const char *dir, bool delRoot) {
    struct dirent dirp;
    struct dirent* result;
    DIR *dp;
    struct stat statbuf;

    if(lstat(dir, &statbuf) < 0)
        return -1;

    if(S_ISDIR(statbuf.st_mode) == 0) {
        //not a directory, remove it directely
        unlink(dir);
        return 0;
    }

    std::string path = dir;
    if(path[path.size() - 1] != DIR_SEPARTOR)
        path = path + DIR_SEPARTOR;

    if((dp = opendir(dir)) == NULL)
        return -1;

    while(true) {
        int rcode = readdir_r(dp, &dirp, &result);
        if(rcode < 0) {
            closedir(dp);
            return -1;
        }

        if(result == NULL)
            break;

        if(strcmp(dirp.d_name, ".") == 0 ||
            strcmp(dirp.d_name, "..") == 0)
            continue;

        std::string newpath = path;
        newpath.append(dirp.d_name);

        if(removeDir(newpath.c_str(), true) < 0) {
            closedir(dp);
            return -1;
        }
    }

    closedir(dp);

    if(delRoot)
        return rmdir(dir);
    else
        return true;
}

unsigned int fileSize(const char *filename) {
    struct stat f_stat;
    if(stat(filename, &f_stat) == -1)
        return 0;

    return f_stat.st_size;
}

