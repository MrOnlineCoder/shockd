#include <stdio.h>
#include "config_file.h"
#include <stdarg.h>
#include <assert.h>
#include "request.h"
#include "log.h"
#include "time.h"

//This constant defines maximum log calls in i/o queue. When there are more that MAX_LOGS calls, fflush() called
#define MAX_LOGS 16

char* log_levels[] = {
    "->",
    "Info",
    "Warning",
    "Error",
};

FILE* logFile = 0;
int logged = 0;
char* buf;

char* shock_format_time() {
    time_t rawtime;
    time(&rawtime);
    return ctime(&rawtime);
}

static void shock_logprint_time() {
    assert(logFile != 0);
    time_t t = time(NULL);
    struct tm tm = *gmtime(&t);
    fprintf(logFile, "%02d.%02d.%04d %02d:%02d:%02d - "
            , tm.tm_mday
            , tm.tm_mon  + 1
            , tm.tm_year + 1900
            , tm.tm_hour
            , tm.tm_min
            , tm.tm_sec);

}

int shock_log_init(shock_config_t* conf)
{
    //Config: ClearLogs
    if (conf->clearLogs == 1) {
        logFile = fopen (conf->logFile,"w");
        if (!logFile) {
            return -1;
        }
        fclose(logFile);
    }

    logFile = fopen (conf->logFile,"a");
    if (!logFile) {
        return -1;
    }
    buf = malloc(128);
    if (!buf) return -1;
    return 0;
}

void shock_log(int level, char* format, ...)
{
    logged++;
    shock_logprint_time();
    fprintf(logFile, "[%s] ", log_levels[level]);
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
    shock_logprint_time();
    fprintf(logFile, "%s %s HTTP/1.%d (%s) \n", httpMethods[req->method], req->route, req->version, req->agent);
    if (logged == MAX_LOGS) {
        fflush(logFile);
        logged = 0;
    }
}
