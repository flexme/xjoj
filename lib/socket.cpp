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

#include "log.h"
#include "file.h"
#include "socket.h"
#include "util.h"
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>

//connect to server
//return whether the connection is successful
bool Socket::connect(std::string host, int port) {
    if(_status != Socket::Closed)
        return false;

    struct sockaddr_in sockstruct;
    bzero(&sockstruct, sizeof(sockstruct));

    if((_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        BLOG("Socket::connect() error", SYSCALL_ERROR);
        return false;
    }

    sockstruct.sin_family = AF_INET;
    sockstruct.sin_port = htons(port);
    if(inet_pton(AF_INET, host.c_str(), &sockstruct.sin_addr) < 0) {
        BLOG("Socket::connect(): inet_pton() error", SYSCALL_ERROR);
        this->close();
        return false;
    }

    if(::connect(_socket_fd, (sockaddr*)&sockstruct, sizeof(sockstruct)) < 0) {
        BLOG("Socket::connect(): connect() error", SYSCALL_ERROR);
        this->close();
        return false;
    }

    _block = true;
    _status = Socket::Connected;
    read_func = ::readn;
    write_func = ::writen;

    this->p = this->buffer + this->bufferSize;

    return true;
}

Socket::Socket(int socket_fd, bool block) {
    _status = Socket::Connected;
    _socket_fd = socket_fd;
    bufferSize = sizeof(bufferSize);
    _block = block;
    if(_block) {
        read_func = ::readn;
        write_func = ::writen;
    }else{
        read_func = ::nonBlockReadn;
        write_func = ::nonBlockWriten;
    }
    
    this->p = this->buffer + this->bufferSize;
}

//set the socket to nonblock
int Socket::setNonBlock() {
    if(_status == Socket::Closed)
        return -1;

    int ret = fcntl(_socket_fd, F_SETFL, fcntl(_socket_fd, F_GETFL) | O_NONBLOCK);
    if(ret == 0) {
        _block = false;
        read_func = ::nonBlockReadn;
        write_func = ::nonBlockWriten;
    } else
        BLOG("Socket::setNonBlock() error", SYSCALL_ERROR);

    return ret;
}

//set the socket to block
int Socket::setBlock() {
    if(_status == Socket::Closed)
        return -1;

    int ret = fcntl(_socket_fd, F_SETFL, fcntl(_socket_fd, F_GETFL) | ~O_NONBLOCK);
    if(ret == 0) {
        _block = true;
        read_func = ::readn;
        write_func = ::writen;
    } else
        BLOG("Socket::setBlock() error", SYSCALL_ERROR);

    return ret;
}

//close the connection
int Socket::close() {
    if(_status == Socket::Closed)
        return 0;

    int ret = ::close(_socket_fd);
    _status = Socket::Closed;
    _socket_fd = -1;
    return ret;
}

//read a char from the socket
int Socket::nextChar() {
    if(this->p - this->buffer >= this->bufferSize) {
        fd_set fd_read;
        FD_ZERO(&fd_read);
        FD_SET(_socket_fd, &fd_read);
        while(true) {
            int nret = select(_socket_fd + 1, &fd_read, 0, 0, 0);
            if(nret < 0) {
                if(errno == EINTR)
                    continue;

                BLOG("Socket::nextChar(): select error", SYSCALL_ERROR);

                return -1;
            }

            break;
        }

        this->bufferSize = read_func(_socket_fd, buffer, sizeof(buffer));

        if(int(this->bufferSize) <= 0) {
            BLOG("Socket::nextChar(): Fail to read from socket", SYSCALL_ERROR);
            return -1;
        }

        if(int(this->bufferSize) == 0)
            return 0;

        this->p = this->buffer;
    }
    return *this->p;
}

//read a line from the socket
int Socket::readLine(char *buf) {
    if(_status != Socket::Connected)
        return -1;

    char *ptr = buf;

    while(true) {
        int ret = nextChar();
        if(ret == -1) {
            //connection is closed
            close();
            return -1;
        }
        p++;
        if(ret == '\n')
            break;

        *ptr++ = ret;
    }
    *ptr = 0;
    return ptr - buf;
}

int Socket::read(char *buf, size_t count) {
    if(_status != Socket::Connected)
        return -1;

    char *ptr = buf;
    while(count) {
        int ret = nextChar();
        if(ret == -1) {
            //connecton is closed
            close();
            return -1;
        }
        p++;
        *ptr++ = ret;
        count--;
    }
    return ptr - buf;
}

int Socket::write(const char* buf, size_t count) {
    if(_status != Socket::Connected)
        return -1;

    ssize_t wn = write_func(_socket_fd, buf, count);
    if(wn != count) {
        BLOG("Socket::write() error:", SYSCALL_ERROR);
        this->close();
        return -1;
    }

    return wn;
}

