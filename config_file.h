#ifndef SHOCK_CONFIG_H
#define SHOCK_CONFIG_H

#include "inttypes.h"
#include "stdio.h"

#define SHOCK_DEFAULT_PORT 80
#define SHOCK_DEFAULT_ROOT "htdocs"
#define SHOCK_DEAFULT_CLEARLOGS 0
#define SHOCK_DEAFULT_ETAGCACHE 1
#define SHOCK_DEFAULT_LOG "shockd.log"

typedef struct {
    uint16_t port;
    char root[FILENAME_MAX];
    int clearLogs;
    int etagCache;
    char logFile[FILENAME_MAX];
} shock_config_t;

typedef struct {
    char name[256];
    char val[512];
} shock_config_keypair_t;

int shock_parse_config(shock_config_t* conf, char* filename);
void shock_default_config(shock_config_t* conf);
int shock_parse_config_token(shock_config_t* conf, shock_config_keypair_t entry);

#endif // SHOCK_CONFIG_H
