#ifndef SHOCK_ERROR_H
#define SHOCK_ERROR_H

#include <SDL2/SDL_net.h>
#include "request.h"

/*
    This method are used when no custom error pages specified.
*/

int shock_error_badrequest(TCPsocket sock, shock_request_t* req);
int shock_error_internal(TCPsocket sock, char* msg);
int shock_error_notfound(TCPsocket sock, char* msg);

int shock_error_respone(TCPsocket sock, int errorCode, char* errorStatus, char* errorMsg);

#endif // SHOCK_ERROR_H
