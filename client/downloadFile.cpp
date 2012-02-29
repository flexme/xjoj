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
#include "socket.h"
#include "file.h"
#include "downloadFile.h"
#include "compress.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct UncompressFunc {
    int fd;
    z_stream strm;

    UncompressFunc(std::string filename) {
        fd = -1;
        fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if(fd < 0)
            BLOG("downloadFile() can not create file " + filename, SYSCALL_ERROR);

        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        inflateInit(&strm);
    }

    ~UncompressFunc() {
        ::close(fd);
        inflateEnd(&strm);
    }

    void operator()(const unsigned char *buf, int len) {
        if(fd >= 0)
            ::writen(fd, (const char*)buf, len);
    }
};

static bool downloadZlibFile(Socket *tcpSocket, std::string filename) {
    char buf[CHUNK];
    UncompressFunc *func = new UncompressFunc(filename.c_str());
    while(1) {
        tcpSocket->readLine(buf);
        int len = atoi(buf);
        if(len <= 0)
            break;

        tcpSocket->read(buf, len);
        uncompress(&func->strm, (const unsigned char*)buf, len, func); 
    }
    delete func;
}

bool downloadFile(Socket *tcpSocket, std::string filename, bool zlib) {
    if(zlib) {
        return downloadZlibFile(tcpSocket, filename);
    }

    char buf[4096];
    int wn = tcpSocket->readLine(buf);
    if(wn <= 0) {
        tcpSocket->close();
        return false;
    }

    int code_len = atoi(buf);
    if(code_len <= 0) {
        return false;
    }

    int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if(fd < 0) {
        BLOG("downloadFile() can not create file " + filename, SYSCALL_ERROR);
        return false;
    }

    while(code_len > 0) {
        int count_read = sizeof(buf);
        if(code_len < count_read) 
            count_read = code_len;

        tcpSocket->read(buf, count_read);
        buf[count_read] = 0;
        code_len -= count_read;
        int nw = writen(fd, buf, count_read);
        if(nw != count_read) {
            BLOG("downloadFile() write error", SYSCALL_ERROR);
            close(fd);
            return false;
        }
    }

    close(fd);
    return true;
}

