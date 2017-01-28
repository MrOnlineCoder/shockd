#ifndef SHOCK_REQUEST_H
#define SHOCK_REQUEST_H
#include <stddef.h>
#include "server_defines.h"

/*
 HTTP method constants
*/

#define HTTP_GET 0
#define HTTP_POST 1

#define HTTP_HEADER(x) char x[HEADER_MAX]

static char* httpMethods[] = {
    "GET",
    "POST"
};

typedef struct {
    int version; //Version of HTTP protocol (0 means HTTP/1.0, 1 means HTTP/1.1)
    int method; //HTTP method
    char route[2048]; // The route path (/example/route)
    char errorMsg[256]; // Error msg, it is contained in request (yea..)
    //===
    //Request headers
    //===
    HTTP_HEADER(etag);
    HTTP_HEADER(host);
    HTTP_HEADER(agent);
    HTTP_HEADER(accept);
} shock_request_t;

typedef struct {
    char name[256];
    char value[512];
} shock_header_t;

/** \brief Parses HTTP request from buffer and updates the shock_request_t
 *
 * \param req is shock_request_t sturct pointer
 * \param reqBuffer is pointer to buffer with TCP request from client
 * \param buffSize is size_t and it is size of buffer
 * \return 0 on success, -1 on error
 *
 */
 int shock_parse_request(shock_request_t* req, char* reqBuffer, size_t buffSize);


/** \brief Parses first line of HTTP request (method, route, HTTP version)
 *
 * \param req is shock_request_t sturct pointer
 * \param firstline is first line of buffer :D
 * \return 0 on success, -1 on error
 *
 */
int shock_parse_request_firstline(shock_request_t* req, char* firstline, size_t lineSize);

/** \brief Parses header line of HTTP request
 *
 * \param req is shock_request_t sturct pointer
 * \param line is the line to be parsed
 * \param lineSize is size of line
 * \return 0 on success, -1 on error, 1 when all headers are parsed
 *
 */
int shock_parse_request_header(shock_request_t* req, char* line, size_t lineSize);




#endif

