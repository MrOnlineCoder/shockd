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
    /*
    //If cache entry was found, then return it
    for (int i=0;i<cacheSize;i++) {
        if (strcpy(cache[i].filename, filename) == 0) {
            sprintf(buf, "%d", cache[i].lm);
            return buf;
        }
    }
    //If not, add it and return the value;*/
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
