#include "request.h"
#include <stdio.h>
#include <string.h>

int isspace(char c) {
    if (c == ' ') {
        return 1;
    } else {
        return 0;
    }
}

int shock_parse_request(shock_request_t* req, char* reqBuffer, size_t buffSize)
{
    char firstline[1024];
    for(int i=0;i<buffSize;i++) {
        if (reqBuffer[i]!='\n') {
            firstline[i] = reqBuffer[i];
        } else {
            int errorCode = shock_parse_request_firstline(req, &firstline, strlen(firstline));
            if (errorCode == -1) return -1;
            break;
        }
    }

    return 0;
}

int shock_parse_request_firstline(shock_request_t* req, char* firstline, size_t lineSize)
{
    char method[32] = " ";
    char path[1024] = " ";
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
