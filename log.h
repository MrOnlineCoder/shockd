#ifndef SHOCK_LOG_H
#define SHOCK_LOG_H

#include "config_file.h"
#include "request.h"

#define SHOCK_ACCESS 0
#define SHOCK_INFO 1
#define SHOCK_WARNING 2
#define SHOCK_ERROR 3

int shock_log_init(shock_config_t* conf);
void shock_log(int level, char* fmt, ...);
void shock_log_access(shock_request_t* req);
char* shock_format_time();
void shock_log_raw(char* format, ...);

#endif
