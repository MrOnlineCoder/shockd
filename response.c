#include "response.h"
#include "server_defines.h"
//#include <SDL2/SDL_net.h>
#include <stdio.h>

#include "base.h"

void shock_serve_file(SOCKET sock, char* filename)
{
    long fsize;
    FILE *fp = fopen(filename, "rb");
    if (!fp){
        shock_error_notfound(sock, "Requested file cannot be found.");
        return;
    }

    shock_send_responseline(sock, 200, "OK", 1);
    shock_default_headers(sock);
    long buflen;
    char buf[2048];
    while (1) {
        buflen = fread(buf, 1, sizeof(buf), fp);
        if (buflen < 1) {
            if (!feof(fp)) {
                printf("Internal Server Error: An error occured while reading file: %s \n", filename);
                shock_error_internal(sock, "Failed to read data from requested file!");
                break;
            }
            break;
        }
        send(sock, buf, buflen, 0);
    }
    fclose(fp);
    free(buf);
}


void shock_default_headers(SOCKET sock)
{
    char buf[1024];
    sprintf(buf, "Server: %s\r\n", SERVER_STRING);
    //SDLNet_TCP_Send(sock, buf, strlen(buf));
    send(sock, buf, strlen(buf), 0);
    //sprintf(buf, "Content-Type: text/html\r\n");
    //SDLNet_TCP_Send(sock, buf, strlen(buf));
    sprintf(buf, "\r\n"); // DO NOT DELETE! This seperates the body and headers so!
    //SDLNet_TCP_Send(sock, buf, strlen(buf));
    send(sock, buf, strlen(buf), 0);
}


void shock_send_responseline(SOCKET sock, int status, char* statusMsg, int httpVer)
{
    char buf[1024];
    sprintf(buf, "HTTP/1.%d %d %s\r\n", httpVer, status, statusMsg);
    //SDLNet_TCP_Send(sock, buf, strlen(buf));
    send(sock, buf, strlen(buf), 0);
}
