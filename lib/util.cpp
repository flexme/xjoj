/*
 * util.cpp 一些常用函数的实现
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

#include <string.h>
#include "util.h"
#include "log.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <cstdlib>

sighandler_t installSignalHandler(int signal, sighandler_t handler, int flags, sigset_t mask) {
	struct sigaction act, oact;
	act.sa_handler = handler;
	act.sa_mask = mask;
	act.sa_flags = flags;

	if(signal == SIGALRM) {
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;		    //SunOs
#endif
	} else {
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;			//SVR4,4.3+BSD
#endif
	}

	if(sigaction(signal, &act, &oact) < 0) {
		return SIG_ERR;
	}

	return oact.sa_handler;
}

sighandler_t installSignalHandler(int signal, sighandler_t handler, int flags) {
	sigset_t mask;
	sigemptyset(&mask);
	return installSignalHandler(signal, handler, flags, mask);
}

int CreateServerSocket(int* port) {
    int sock = socket(PF_INET, SOCK_STREAM, 6);
    if (sock == -1) {
        BLOG("CreateServerSocket()::Fail to create socket:", SYSCALL_ERROR);
        return -1;
    }

    int optionValue = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) == -1) {
        BLOG("CreateServerSocket::Fail to set socket option:", SYSCALL_ERROR);
        close(sock);
        return -1;
    }

    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(*port);
    if (bind(sock, (struct sockaddr*)&address, sizeof(address)) == -1) {
        BLOG("CreateServerSocket::Fail to bind:", SYSCALL_ERROR);
        close(sock);
        return -1;
    }

    if (listen(sock, 32) == -1) {
        BLOG("CreateServerSocket::Fail to listen", SYSCALL_ERROR);
        close(sock);
        return -1;
    }
    socklen_t len = sizeof(address);
    if (getsockname(sock, (struct sockaddr*)&address, &len) == -1) {
        BLOG("CreateServerSocket::Fail to get socket port:", SYSCALL_ERROR);
        close(sock);
        return -1;
    }

    *port = ntohs(address.sin_port);
    return sock;
}

unsigned int get_myip() {
    int i;
    int tmpsocket;
    struct ifconf ifc;
    char ifcbuf[8 * sizeof(struct ifreq)];
    struct ifreq *ifr;
    struct sockaddr_in *paddr;
    unsigned int ret_ip;

    bzero(ifcbuf, sizeof(ifcbuf));

    ifc.ifc_len = sizeof(ifcbuf);
    ifc.ifc_buf = ifcbuf;

    tmpsocket = socket(AF_INET, SOCK_STREAM, 0);
    i = ioctl(tmpsocket, SIOCGIFCONF, (void*)&ifc);
    close(tmpsocket);

    if(i < 0) {
        return -1;
    }

    for(ifr = ifc.ifc_req, i = 0; i < ifc.ifc_len / sizeof(struct ifreq); i++, ifr++) {
        paddr = (struct sockaddr_in*)&(ifr->ifr_addr);
        ret_ip = paddr->sin_addr.s_addr;
    }

    return ret_ip;
}

int write_pid(const char* pid_filename) {
    int fd;
    if((fd = open(pid_filename, O_WRONLY | O_TRUNC | O_CREAT, S_IWUSR)) < 0) {
        LOG(SYSCALL_ERROR) << "open pidfile faild";
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;

    if(fcntl(fd, F_SETLK, &lock) == -1) {
        int err = errno;
        LOG(SYSCALL_ERROR) << "fcntl faild\n";

        if(err == EAGAIN) {
            LOG(ERROR) << "Another problem instance is running now!\n";
        }
        return -1;
    }

    return 0;
}

int setLimit(int res, unsigned int limit) {
    struct rlimit t;
    t.rlim_max = limit + 1;
    t.rlim_cur = limit;
    return setrlimit(res, &t);
}

void daemon_init() {
    switch(fork()) {
        case -1:
            exit(2);

        case 0:
            break;

        default:
            exit(0);
    }
    setsid();
    switch(fork()) {
        case -1:
            exit(2);

        case 0:
            break;

        default:
            exit(0);
    }
}

