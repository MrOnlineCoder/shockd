#include <stdio.h>
#include "config_file.h"
#include <stdarg.h>
#include "request.h"
#include "log.h"
#include "time.h"

#define MAX_LOGS 16

char* log_levels[] = {
    "->",
    "Info",
    "Warning",
    "Error",
};

FILE* logFile;
int logged = 0;

char* shock_format_time()
{
    time_t t = time(NULL);
    struct tm tm = *gmtime(&t);
    char buf[128] = "";
    sprintf(buf, "%d.%d.%d %d:%d:%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return strdup(buf);
}

int shock_log_init(shock_config_t* conf)
{
    logFile = fopen (conf->logFile,"a");
    if (!logFile) {
        return -1;
    }
    return 0;
}

void shock_log(int level, char* format, ...)
{
    logged++;
    fprintf(logFile, "%s: [%s] ", shock_format_time(), log_levels[level]);
    va_list args;
    va_start (args, format);
    vfprintf (logFile, format, args);
    va_end (args);
    fprintf(logFile, "\n"); //It also append newline character, because writing it in every call annoys
    if (logged == MAX_LOGS) {
        fflush(logFile);
        logged = 0;
    }
}

void shock_log_raw(char* format, ...)
{
    va_list args;
    va_start (args, format);
    vfprintf (logFile, format, args);
    va_end (args);
}



void shock_log_access(shock_request_t* req)
{
    logged++;
    fprintf(logFile, "%s: %s %s HTTP/1.%d (%s) \n", shock_format_time(), httpMethods[req->method], req->route, req->version, req->agent);
    if (logged == MAX_LOGS) {
        fflush(logFile);
        logged = 0;
    }
}




