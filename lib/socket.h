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

#ifndef __SOCKET_H
#define __SOCKET_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <string>

typedef ssize_t (*read_write_func)(int, const void*, size_t);

class Socket {
public:
    enum SocketStatus {
        Closed,
        Connected
    };

    Socket() {
        _status = Socket::Closed;
        _socket_fd = -1;
        bufferSize = sizeof(buffer);
    }

    Socket(int socket_fd, bool block);

    ~Socket() {
        this->close();
    }

    Socket::SocketStatus status() const {
        return _status;
    }

    bool isBlock() const {
        return _block;
    }

    bool connect(std::string host, int port);

    int setNonBlock();

    int setBlock();

    int close();
    
    int readLine(char *);

    int read(char *, size_t);
    int write(const char *, size_t);

    int totBytes() const {
        return buffer + bufferSize - p;
    }

private:
    int nextChar();

private:
    SocketStatus _status;

    int _socket_fd;

    bool _block;

    unsigned char *p;
    unsigned char buffer[1024];
    int bufferSize;

    read_write_func read_func;
    read_write_func write_func;

    Socket(const Socket&);
    Socket& operator=(const Socket&);
    const Socket& operator=(const Socket&) const;
};

#endif

