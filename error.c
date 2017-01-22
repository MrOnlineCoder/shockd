#include "base.h"

#include "server_defines.h"
#include "request.h"

int shock_error_respone(SOCKET sock, int errorCode, char* errorStatus, char* errorMsg)
{
    char buf[1024] = " "; // We must use it to avoid garbage in string

    sprintf(buf, "HTTP/1.1 %d %s\r\n", errorCode, errorStatus);
    send(sock, buf, strlen(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(sock, buf, strlen(buf), 0);
    sprintf(buf, "Connection: close\r\n");
    send(sock, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(sock, buf, strlen(buf), 0);
    sprintf(buf, "<title>%s</title>", errorStatus);
    send(sock, buf, strlen(buf), 0);
    sprintf(buf, "<h1>Error %i %s</h1>\r\n", errorCode, errorStatus);
    send(sock, buf, strlen(buf), 0);
    sprintf(buf, "<p>%s</p>\r\n", errorMsg);
    send(sock, buf, strlen(buf), 0);
    sprintf(buf, "<hr>\r\n", errorMsg);
    send(sock, buf, strlen(buf), 0);
    sprintf(buf, "<i>%s</i>", SERVER_STRING);
    send(sock, buf, strlen(buf), 0);
    closesocket(sock);
    return 0;
}


int shock_error_badrequest(SOCKET sock, shock_request_t* req)
{
    shock_error_respone(sock, 401, "Bad Request", req->errorMsg);
    return 0;
}


int shock_error_internal(SOCKET sock, char* msg)
{
    shock_error_respone(sock, 500, "Internal Server Error", msg);
    return 0;
}

int shock_error_notfound(SOCKET sock, char* msg)
{
    shock_error_respone(sock, 404, "Not Found", msg);
    return 0;
}



