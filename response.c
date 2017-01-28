#include "response.h"
#include "server_defines.h"
#include <stdio.h>
#include <time.h>

#include "base.h"

char* shock_date_now() {
  char buf[256];
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tm);
  return buf;
}

void shock_response_send_header(SOCKET sock, char* header, char* value)
{
    char buf[1024];
    sprintf(buf, "%s: %s\r\n", header, value);
    send(sock, buf, strlen(buf), 0);
}



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
    shock_response_send_header(sock, "Etag", shock_cache_get(filename));
    shock_response_end_headers(sock);
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
}

void shock_response_end_headers(SOCKET sock)
{
    char buf[16];
    sprintf(buf, "\r\n"); // DO NOT DELETE! This seperates the body and headers so!
    send(sock, buf, strlen(buf), 0);
}


void shock_default_headers(SOCKET sock)
{
    shock_response_send_header(sock, "Date", shock_date_now());
    shock_response_send_header(sock, "Server", SERVER_STRING);
}


void shock_send_responseline(SOCKET sock, int status, char* statusMsg, int httpVer)
{
    char buf[1024];
    sprintf(buf, "HTTP/1.%d %d %s\r\n", httpVer, status, statusMsg);
    send(sock, buf, strlen(buf), 0);
}

void shock_response_send_header_i(SOCKET sock, char* header, int value)
{
    char buf[1024];
    sprintf(buf, "%s: %d\r\n", header, value);
    send(sock, buf, strlen(buf), 0);
}


void shock_response_notmodified(SOCKET sock)
{
    shock_send_responseline(sock, 304, "Not Modified", 1);
    shock_default_headers(sock);
    shock_response_end_headers(sock);
}

