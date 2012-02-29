/*
 * executer.cpp
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

#include "executer.h"
#include "traceProcess.h"
#include "sqlError.h"
#include "judgeStatus.h"
#include "log.h"
#include "config.h"
#include "stringUtil.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void sigalrm_handler(int signo) {
    return;
}

Executer::~Executer() {
#ifndef TEST
    unlink(Config::getInstant()->getUserOut(runId).c_str());
#endif
}

void Executer::init() {
    _result = TraceProcess::NORMAL;
    _runnedMemory = 0;
    _runnedTime = 0;
}

