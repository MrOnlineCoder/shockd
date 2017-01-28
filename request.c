#include "request.h"
#include <stdio.h>
#include <string.h>
#include "server_defines.h"

int isspace(char c) {
    if (c == ' ') {
        return 1;
    } else {
        return 0;
    }
}

int shock_parse_request(shock_request_t* req, char* reqBuffer, size_t buffSize)
{
    char currentLine[1024] = "";
    int parsedFirstLine = 0;
    int lineChar = 0; //Counter for currentLine
    for(int i=0;i<buffSize;i++) {
        if (reqBuffer[i]!='\n') {
            currentLine[lineChar] = reqBuffer[i];
            lineChar++;
            continue;
        }
        if (parsedFirstLine == 0) {
            parsedFirstLine = 1;
            int errorCode = shock_parse_request_firstline(req, &currentLine, strlen(currentLine));
            if (errorCode == -1) return -1;
            continue;
        }
        int headerCode = shock_parse_request_header(req, &currentLine, strlen(currentLine));
        if (headerCode == 1) printf("Parsing done.\n");
        memset(currentLine, 0, sizeof(currentLine));
        lineChar=0;
    }

    return 0;
}

int shock_parse_request_firstline(shock_request_t* req, char* firstline, size_t lineSize)
{
    char method[32] = " ";
    char path[ROUTE_MAX] = " ";
    char version[32] = " ";

    int i = 0; // this is total counter for line
    int j = 0; // this is counter for current token (method/url/version)


    //Parse the method name
    while(isspace(firstline[j]) == 0 && j < lineSize-1){
        if (j == sizeof(method)-1) {
            strcpy(req->errorMsg, "Your browser sent too long method name."); //Probably, browser won't sent invalid requests, but security is required
            return -1;
        }
        method[j] = firstline[j];
        j++;
    }
    //We should add null-terminator to each token
    method[j] = '\0';
    i = j;
    j=0;
    i++;

    //Parse route itself
    while(isspace(firstline[i]) == 0 && i < lineSize-1) {
        if (j == sizeof(path)-1) {
            strcpy(req->errorMsg, "Too long URL.");
            return -1;
        }
        path[j] = firstline[i];
        j++;
        i++;
    }
    path[j] = '\0';
    j=0;
    i++;

    //Parse http version
    while(isspace(firstline[i]) == 0 && i < lineSize-1) {
        if (j == sizeof(version)-1) {
            strcpy(req->errorMsg, "Too long HTTP version token.");
            return -1;
        }
        if (firstline[i] == '\r' || firstline[i] == '\n') break; //we should handle both cases
        version[j] = firstline[i];
        j++;
        i++;
    }
    version[j] = '\0';
    /*printf("Request method: %s\n", method);
    printf("Request path: %s\n", path);
    printf("Request version: <%s>\n", version);*/

    //#############################
    //Now check if request is valid
    //#############################

    //1.Check method
    if (strcmp(method, "GET") == 0) {
        req->method = HTTP_GET;
    } else if (strcmp(method, "POST") == 0) {
        req->method = HTTP_POST;
    } else {
        strcpy(req->errorMsg, "Your browser sent request with unsupported HTTP method");
        return -1;
    }

    //We don't check route, no rules here :)
    strcpy(req->route, path);

    //2.Check version
    if (strcmp(version, "HTTP/1.1") == 0) {
        req->version = 1;
    } else if (strcmp(version, "HTTP/1.0") == 0) {
        req->version = 0;
    } else {
        strcpy(req->errorMsg, "Your browser used unsupported HTTP version in request");
        return -1;
    }

    return 0;
}

int shock_parse_request_header(shock_request_t* req, char* line, size_t lineSize)
{
    //
    // YES! It is awful hard-coded check! If you have an idea how to implement this better, create a pull request
    //

    char token[256] = "";
    int j = 0;
    int phase = 0;
    shock_header_t header;
    for (int i=0;i<lineSize;i++) {
        if (isspace(line[i]) == 1) {
            continue;
        }
        if (line[i]==':' && phase == 0) {
            strcpy(header.name, token);
            memset(token, 0, sizeof(token));
            phase = 1;
            j = 0;
            continue;
        }

        token[j] = line[i];
        j++;
    }
    strcpy(header.value, token);

    if (strcasecmp(header.name, "If-None-Match") == 0) {
        strcpy(req->etag, header.value);
        return 0;
    }

    if (strcasecmp(header.name, "User-Agent") == 0) {
        strcpy(req->agent, header.value);
        return 0;
    }

    //printf("[Request] Warning: unsupported header: %s\n", header.name);
    return 0;
}

