#ifndef SHOCK_ERROR_H
#define SHOCK_ERROR_H

//#include <SDL2/SDL_net.h>
#include "request.h"
#include "base.h"

/*
    This method are used when no custom error pages specified.
*/

int shock_error_badrequest(SOCKET sock, shock_request_t* req);
int shock_error_internal(SOCKET sock, char* msg);
int shock_error_notfound(SOCKET sock, char* msg);

int shock_error_respone(SOCKET sock, int errorCode, char* errorStatus, char* errorMsg);

#endif // SHOCK_ERROR_H
