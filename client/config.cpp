/*
 * config.cpp
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

#include "config.h"
#include "util.h"
#include "file.h"
#include "md5.h"
#include "log.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>

Config *Config::config = NULL;

//the file name of config file
const std::string Config::configFile = "etc/client_config.ini";

void printUsage() {
	printf("Usage:  judge  [--queue=<queue address>] \n"
								 "[--uid=<uid>] "
								 "[--gid=<gid>] \n"
                                 "[-s] sync data \n"
                                 "[-h] help \n");
}

Config::Config() {
    //the default value of parameters
    password = "";
    job_uid = getuid();
    job_gid = getgid();
    dataInPath = "data/in/";
    dataOutPath = "data/out/";
    spjPath = "data/spj/";
    tmpSourcePath = "data/src/";
    tmpUserPath = "data/tmp/";
    threadNum = 2;
    needto_sync_data = false;

    char buf[512];
    getcwd(buf, 512);
    rootPath = buf;
    rootPath = checkDir(rootPath);

    logRoot = rootPath;
    logFileName = DEFAULT_LOGFILE;
    dataInPath = checkDir(dataInPath);
    dataOutPath = checkDir(dataOutPath);
    spjPath = checkDir(spjPath);
    tmpSourcePath = checkDir(tmpSourcePath);
    tmpUserPath = checkDir(tmpUserPath);

    iniError = !this->initConfig();

    myip = get_myip();
}

bool Config::initConfig(int argc, char *argv[]) {
    //the global variable optind need to be initialnized
    optind = 0;

    struct option options[] = {
        {"queue", 1, 0, 'q'},
        {"uid", 1, 0, 'u'},
        {"gid", 1, 0, 'g'},
        {0, 0, 0, 0}
    };

    const char* const short_options = "sh";

    const char *p;
    char buf[100];

    for(;;) {
        switch(getopt_long(argc, argv, short_options, options, NULL)) {
            case 'q':
                //set server address
                p = strstr(optarg, ":");
                if(!p) {
                    sprintf(buf, "Invalid server address %s", optarg);
                    errMesg = std::string(buf);
                    return false;
                }
                serverAddress.first = trim(std::string(optarg, p - optarg));
                serverAddress.second = atoi(p + 1);
                if(serverAddress.second < 1 || serverAddress.second > 65535) {
                    errMesg = std::string("server port number must between 1 to 65535");
                    return false;
                }
                break;

            case 'u':
                //set job uid
                if(sscanf(optarg, "%d", &job_uid) < 1) {
                    sprintf(buf, "Invalid uid %s", optarg);
                    errMesg = std::string(buf);
                    return false;
                }
                break;

            case 'g':
                //set gob gid
                if(sscanf(optarg, "%d", &job_gid) < 1) {
                    sprintf(buf, "Invalid gid %s", optarg);
                    errMesg = std::string(buf);
                    return false;
                }
                break;

            case 's':
                needto_sync_data = true;
                break;

            case 'h':
                printUsage();
                ::exit(0);
                break;

            case -1:
                return true;

            default:
                errMesg = "Invalid argv";
                return false;;
        }
    }
}

//add a DIR_SEPARATOR at the end of the dir path
inline std::string Config::checkDir(std::string dir) {
    std::string ret = dir;
    if(ret[ret.size() - 1] != DIR_SEPARTOR) {
        ret.push_back(DIR_SEPARTOR);
    }
    return ret;
}

//read information from the config file
bool Config::initConfig() {
    ConfigReader judgeSettings(configFile, ConfigReader::SampleINI);
    int status = judgeSettings.status();
    
    if(status == ConfigReader::OK) {
         password = judgeSettings.getValue("password", password).toString();
         //md5
         password = md5_string(password.c_str());
         job_uid = judgeSettings.getValue("JobUID", job_uid).toInt();
         job_gid = judgeSettings.getValue("JobGID", job_gid).toInt();
         threadNum = judgeSettings.getValue("threads", threadNum).toInt();
         
         if(job_uid == 0) {
            job_uid = getuid();
         }
         
         if(job_gid == 0) {
             job_gid = getgid();
         }
        
         dataInPath = judgeSettings.getValue("dataInPath", dataInPath).toString();
         dataOutPath = judgeSettings.getValue("dataOutPath", dataOutPath).toString();
         spjPath = judgeSettings.getValue("spjPath", spjPath).toString();
         tmpSourcePath = judgeSettings.getValue("tmpSourcePath", tmpSourcePath).toString();
         tmpUserPath = judgeSettings.getValue("tmpUserPath", tmpUserPath).toString();
         logRoot = judgeSettings.getValue("logRoot", logRoot).toString();
         logFileName = judgeSettings.getValue("logFileName", logFileName).toString();

         if(access(dataInPath.c_str(), R_OK) != 0) {
            this->errMesg = std::string("data in path:") + dataInPath + " doesn't exists";
            return false;
         }

         if(access(dataOutPath.c_str(), R_OK) != 0) {
            this->errMesg = std::string("data out path:") + dataOutPath + " doesn't exists";
            return false;
         }

         if(access(spjPath.c_str(), R_OK) != 0 ) {
            this->errMesg = "spj path:" + spjPath + " doesn't exists";
            return false;
         }

         if(access(tmpSourcePath.c_str(), R_OK) != 0) {
            this->errMesg = std::string("tmp source dir:") + tmpSourcePath + " does'nt exists";
            return false;
         }

         if(access(tmpUserPath.c_str(), R_OK) != 0) {
            this->errMesg = std::string("user dir:") + tmpUserPath + " does'nt exists";
            return false;
         }

         if(access(logRoot.c_str(), R_OK) != 0) {
            this->errMesg = std::string("log root dir:" + logRoot + " does'n exists");
            return false;
         }

         this->serverAddress.first = judgeSettings.getValue("server", "").toString();
         this->serverAddress.second = judgeSettings.getValue("port", "1213").toInt();
         
         if(this->serverAddress.second <= 0 || this->serverAddress.second > 65535) {
            this->errMesg = std::string("server port number must between 1 to 65535");
            return false;
         }

         dataInPath = checkDir(dataInPath);
         dataOutPath = checkDir(dataOutPath);
         spjPath = checkDir(spjPath);
         tmpUserPath = checkDir(tmpUserPath);
         tmpSourcePath = checkDir(tmpSourcePath);
         logRoot = checkDir(logRoot);

    } else if(status == ConfigReader::ACCESS_ERROR) {
        //can't access the config file
        this->errMesg = std::string("can't access the conf file ") + configFile;
        return false;
    } else if(status == ConfigReader::BAD_FILE_FORMAT){
        //the format of the config file is error
        this->errMesg = std::string("error format of ") + configFile;
        return false;
    } else {
        //unkonw error
        this->errMesg = "unkonw error of " + configFile;
        return false;
    }

    return true;
}

