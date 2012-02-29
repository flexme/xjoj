/*
 * Sandbox.cpp Sandbox java程序的jni接口实现
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

#include <jni.h>
#include "Sandbox.h"

#include <string>

#include <sys/resource.h>
#include "util.h"
#include "log.h"
#include "stringUtil.h"
#include "makeconfig.h"

static std::string logFileName = combineString(XJOJHOME, "client_log/javalog_", formatTime("%m-%d"));

JNIEXPORT jint JNICALL Java_Sandbox_setLimits(
    JNIEnv* evn, jclass jcls, jint time_limit, jint output_limit, jint file_limit, jint uid, jint gid) {
    if(time_limit % 1000) {
        time_limit = time_limit / 1000 + 1;
    } else {
        time_limit = time_limit / 1000;
    }

    if((time_limit && setLimit(RLIMIT_CPU, time_limit) < 0) || 
        (output_limit && setLimit(RLIMIT_FSIZE, output_limit * 1024) < 0) ||
        (file_limit && setLimit(RLIMIT_NOFILE, file_limit) < 0) ||
        (gid && setgid(gid) == -1) ||
        (uid && setuid(uid) == -1)) {
        return -1;
    }

    return 0;
}

JNIEXPORT void JNICALL Java_Sandbox_closeLog(JNIEnv *, jclass) {
    Log::getInstance(logFileName)->close(); 
}

JNIEXPORT void JNICALL Java_Sandbox_logError(JNIEnv* env, jclass jcls, jstring message) {
    const char* buf  =env->GetStringUTFChars(message, 0);
    Log::getInstance(logFileName)->log(buf, INFO);
}

