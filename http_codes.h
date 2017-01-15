#ifndef SHOCK_HTTPCODES_H
#define SHOCK_HTTPCODES_H

static const char * const responseCodes[RESPONSE_CODES] =
{
    "100 Continue",
    "101 Switching Protocols",
    "102 Processing",
    "200 OK",
    "201 Created",
    "202 Accepted",
    "203 Non-Authoritative Information",
    "204 No Content",
    "205 Reset Content",
    "206 Partial Content",
    "207 Multi-Status",
    "208 Already Reported",
    NULL, /* 209 */
    NULL, /* 210 */
    NULL, /* 211 */
    NULL, /* 212 */
    NULL, /* 213 */
    NULL, /* 214 */
    NULL, /* 215 */
    NULL, /* 216 */
    NULL, /* 217 */
    NULL, /* 218 */
    NULL, /* 219 */
    NULL, /* 220 */
    NULL, /* 221 */
    NULL, /* 222 */
    NULL, /* 223 */
    NULL, /* 224 */
    NULL, /* 225 */
    "226 IM Used",
    "300 Multiple Choices",
    "301 Moved Permanently",
    "302 Found",
    "303 See Other",
    "304 Not Modified",
    "305 Use Proxy",
    NULL, /* 306 */
    "307 Temporary Redirect",
    "308 Permanent Redirect",
    "400 Bad Request",
    "401 Unauthorized",
    "402 Payment Required",
    "403 Forbidden",
    "404 Not Found",
    "405 Method Not Allowed",
    "406 Not Acceptable",
    "407 Proxy Authentication Required",
    "408 Request Timeout",
    "409 Conflict",
    "410 Gone",
    "411 Length Required",
    "412 Precondition Failed",
    "413 Request Entity Too Large",
    "414 Request-URI Too Long",
    "415 Unsupported Media Type",
    "416 Requested Range Not Satisfiable",
    "417 Expectation Failed",
    "418 I'm A Teapot",
    NULL, /* 419 */
    NULL, /* 420 */
    "421 Misdirected Request",
    "422 Unprocessable Entity",
    "423 Locked",
    "424 Failed Dependency",
    NULL, /* 425 */
    "426 Upgrade Required",
    NULL, /* 427 */
    "428 Precondition Required",
    "429 Too Many Requests",
    NULL, /* 430 */
    "431 Request Header Fields Too Large",
    NULL, /* 432 */
    NULL, /* 433 */
    NULL, /* 434 */
    NULL, /* 435 */
    NULL, /* 436 */
    NULL, /* 437 */
    NULL, /* 438 */
    NULL, /* 439 */
    NULL, /* 440 */
    NULL, /* 441 */
    NULL, /* 442 */
    NULL, /* 443 */
    NULL, /* 444 */
    NULL, /* 445 */
    NULL, /* 446 */
    NULL, /* 447 */
    NULL, /* 448 */
    NULL, /* 449 */
    NULL, /* 450 */
    "451 Unavailable For Legal Reasons",
    "500 Internal Server Error",
    "501 Not Implemented",
    "502 Bad Gateway",
    "503 Service Unavailable",
    "504 Gateway Timeout",
    "505 HTTP Version Not Supported",
    "506 Variant Also Negotiates",
    "507 Insufficient Storage",
    "508 Loop Detected",
    NULL, /* 509 */
    "510 Not Extended",
    "511 Network Authentication Required"
};

#endif // SHOCK_HTTPCODES_H

