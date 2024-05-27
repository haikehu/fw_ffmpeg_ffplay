#include "log.h"
#include <mutex>
#include <stdarg.h>

#include <string>

#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

std::mutex mtx;
FILE *fFile = NULL;

static void LogWrite(char *str,int size)
{
    if(fFile)
    {
        fwrite(str,1, size, fFile);
        fflush(fFile);
    }
}

void LogInit()
{
    fFile = fopen("log.txt","w+");
}

void Serialize (const char* fmt,...)
{
    char buf[1024] = {0};
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t),"%Y-%m-%d %H.%M.%S") << " -- ";
    std::string fmt_str;// = ss.str() ;
    fmt_str += fmt;
    fmt_str += "\n";
    mtx.lock();
    va_list arglist;
    va_start(arglist, fmt);
    //vfprintf(stderr, fmt_str.c_str ), arglist);
    vsnprintf(buf,sizeof(buf), fmt_str.c_str(), arglist);
    //vsnprintf(bufsizeof(buf)，arglist):
    printf("%s",buf);
    LogWrite(buf,strlen(buf));
    va_end(arglist);
    mtx.unlock();
}
