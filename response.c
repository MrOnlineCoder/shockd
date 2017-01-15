#include "response.h"
#include "server_defines.h"
#include <SDL2/SDL_net.h>
#include <stdio.h>

void shock_serve_file(TCPsocket sock, char* filename)
{
    char buf[4 * 1024];

    FILE* fid = NULL;
    fid = fopen(filename, "r");

    if (fid == NULL) {
        printf("Internal Server Error: file %s cannot be opened! \n", filename);
        shock_error_internal(sock, "Requested file cannot be opened");
        return;
    }

    shock_send_responseline(sock, 200, "OK", 1);
    shock_default_headers(sock);
    fgets(buf, sizeof(buf), fid);
    while (!feof(fid))
    {
        SDLNet_TCP_Send(sock, buf, strlen(buf));
        fgets(buf, sizeof(buf), fid);
    }
}


void shock_default_headers(TCPsocket sock)
{
    char buf[1024];
    sprintf(buf, "Server: %s\r\n", SERVER_STRING);
    SDLNet_TCP_Send(sock, buf, strlen(buf));
    //sprintf(buf, "Content-Type: text/html\r\n");
    //SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "\r\n"); // DO NOT DELETE! This seperates the body and headers so!
    SDLNet_TCP_Send(sock, buf, strlen(buf));
}


void shock_send_responseline(TCPsocket sock, int status, char* statusMsg, int httpVer)
{
    char buf[1024];
    sprintf(buf, "HTTP/1.%d %d %s\r\n", httpVer, status, statusMsg);
    SDLNet_TCP_Send(sock, buf, strlen(buf));
}
