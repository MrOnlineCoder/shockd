#ifdef _WIN32

#include <sys/types.h>
#include <sys/stat.h>
#define stat _stat
#define STAT_BUF struct _stat

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define STAT_BUF struct stat

#endif

/*
typedef struct {
    char* filename;
    time_t lm;
} shock_cache_entry_t;
*/


time_t shock_cache_getlm(char* filename);
char* shock_cache_get(char* filename);
int shock_cache_compare(char* filename, time_t val);
