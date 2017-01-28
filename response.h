#ifndef SHOCK_RESPONSE_H
#define SHOCK_RESPONSE_H

#include "base.h"


/*
    We don't have struct that represents response like shock_response_t, because response is created and sent usually with 1 function.
    Meanwhile, request is used between different parsing function calls.
*/

void shock_serve_file(SOCKET sock, char* filename);
void shock_default_headers(SOCKET sock);
void shock_send_responseline(SOCKET sock, int status, char* statusMsg, int httpVer);
char* shock_date_now();
void shock_response_send_header(SOCKET sock, char* header, char* value);
void shock_response_send_header_i(SOCKET sock, char* header, int value);
void shock_response_end_headers(SOCKET sock);
void shock_response_notmodified(SOCKET sock);

#endif // SHOCK_RESPONSE_H
