#include <SDL2/SDL_net.h>

#include "server_defines.h"
#include "request.h"

int shock_error_respone(TCPsocket sock, int errorCode, char* errorStatus, char* errorMsg)
{
    char buf[1024] = " "; // We must use it to avoid garbage in string

    sprintf(buf, "HTTP/1.1 %d %s\r\n", errorCode, errorStatus);
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "Connection: close\r\n");
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "\r\n");
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "<title>%s</title>", errorStatus);
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "<h1>Error %i %s</h1>\r\n", errorCode, errorStatus);
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "<p>%s</p>\r\n", errorMsg);
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "<hr>\r\n", errorMsg);
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "<i>%s</i>", SERVER_STRING);
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    SDLNet_TCP_Close(sock);
    return 0;
}


int shock_error_badrequest(TCPsocket sock, shock_request_t* req)
{
    //printf("Error msg: %s \n", req->errorMsg);
    shock_error_respone(sock, 401, "Bad Request", req->errorMsg);
    return 0;
}


int shock_error_internal(TCPsocket sock, char* msg)
{
    shock_error_respone(sock, 500, "Internal Server Error", msg);
    return 0;
}

int shock_error_notfound(TCPsocket sock, char* msg)
{
    shock_error_respone(sock, 404, "Not Found", msg);
    return 0;
}



