#include "cache.h"


time_t shock_cache_getlm(char* filename)
{
    STAT_BUF buf;
    stat(filename, &buf);
    return buf.st_mtime;
}


char* shock_cache_get(char* filename)
{
    time_t time;
    char* buf = malloc(256);
    time = shock_cache_getlm(filename);
    sprintf(buf, "%d", time);
    return buf;
}

int shock_cache_compare(char* filename, time_t val)
{
    if (shock_cache_getlm(filename) != val) {
        return 0;
    } else {
        return 1;
    }
}
