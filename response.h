#ifndef SHOCK_RESPONSE_H
#define SHOCK_RESPONSE_H

#include <SDL2/SDL_net.h>

/*
    We don't have struct that represents response like shock_response_t, because response is created and sent usually with 1 function.
    Meanwhile, request is used between different parsing function calls.
*/

void shock_serve_file(TCPsocket sock, char* filename);
void shock_default_headers(TCPsocket sock);
void shock_send_responseline(TCPsocket sock, int status, char* statusMsg, int httpVer);

#endif // SHOCK_RESPONSE_H
