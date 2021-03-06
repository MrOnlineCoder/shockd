#include <stdio.h>
#include "request.h"
#include "response.h"
#include "error.h"
#include "server_defines.h"
#include "config_file.h"
#include "thread_wrapper.h"
#include <locale.h>

#include "base.h"
#include "cache.h"
#include "log.h"

shock_config_t conf;

void processRequest(SOCKET sock) {
    char buffer[REQUEST_MAX];
    recv(sock, &buffer, sizeof(buffer), 0);
    shock_request_t request;
    //Parse and validate request
    int reqError = shock_parse_request(&request, &buffer, strlen(buffer));
    if (reqError == -1) {
        shock_error_badrequest(sock, &request);
        return;
    }

    shock_log_access(&request);
    //Now serve the file!
    char* route = request.route;
    char lastchar = route[strlen(route)-1];
    if (lastchar == '/') {
        strcat(route, "index.html");
    }
    char filename[FILENAME_MAX] = "";
    sprintf(filename, "%s%s", conf.root, route);
    if (conf.etagCache == 1 && request.etag[0] != 0) {
        char *ptr;
        time_t res = strtoll(request.etag, &ptr, 10);
        if (shock_cache_compare(filename, res) == 1) {
            shock_response_notmodified(sock);
        } else {
        shock_serve_file(sock, filename);
        }
    } else {
        shock_serve_file(sock, filename);
    }

    closesocket(sock);
}

int initSockets() {
#ifdef _WIN32
        WORD version_wanted = MAKEWORD(1,1);
        WSADATA wsaData;

        if ( WSAStartup(version_wanted, &wsaData) != 0 ) {;
            return -1;
        }
#endif // _WIN32
    return 0;
}

int main(int argc, char* argv[]) {
    printf("*** shockd server by MrOnlineCoder ***\n");
    printf("Initializing...\n\n");

    if (initSockets() == -1) {
        printf("[Fatal Error] Cannot init Winsock!\n");
        return 1;
    }

    //setlocale(LC_ALL, "");


    //IPaddress ip;

    printf("Applying configuration from shockd.conf file...\n\n");
    shock_config_t config;
    shock_default_config(&config);
    if (shock_parse_config(&config, "shockd.conf") == -1) {
        printf("[Fatal Error] cannot parse config!");
        return 1;
    }
    conf = config;
    printf("[Config] Port: %d\n", conf.port);
    printf("[Config] Server Root: %s\n", conf.root);
    printf("[Config] ClearLogs: %d\n", conf.clearLogs);
    printf("[Config] ETag Cache: %d\n", conf.etagCache);
    printf("[Config] Log File: %s\n", conf.logFile);

    if (shock_log_init(&conf) == -1) {
        printf("[Fatal Error] Cannot init logging system!\n");
        return 1;
    }

    printf("\nResolving IP...\n\n");

    SOCKET server;

    struct sockaddr_in serverIP;
    server = socket(AF_INET , SOCK_STREAM , 0);
    if (server == -1)
    {
       shock_log(SHOCK_ERROR, "Cannot create server socket!");
       printf("[Fatal Error] Cannot create server socket!");
       return 1;
    }

    //Prepare the sockaddr_in structure
    serverIP.sin_family = AF_INET;
    serverIP.sin_addr.s_addr = INADDR_ANY;
    serverIP.sin_port = htons( conf.port );

    if( bind(server,(struct sockaddr *)&serverIP , sizeof(serverIP)) < 0)
    {
        shock_log(SHOCK_ERROR, "Cannot bind socket to port!!");
        printf("[Fatal Error] Cannot bind socket to port! \n");
        return 1;
    }

    printf("Starting server...\n\n");

    if (listen(server, 5) == SOCKET_ERROR) {
        printf("[Fatal Error] Cannot start listening on given port! \n");
        return 1;
    }


    SOCKET client;
    struct sockaddr_in clientIP;
    size_t c = sizeof(struct sockaddr_in);


    shock_log_raw("-----------------------------------------------\n\n");
    shock_log_raw("NEW SERVER SESSION STARTED AT %s\n", shock_format_time());
    shock_log_raw("-----------------------------------------------\n");
    shock_log(SHOCK_INFO, "Server started!");
    printf("[!] Done. Waiting for incoming connections...\n");
    while (1) {
        client=accept(server, (struct sockaddr *)&clientIP, (socklen_t*)&c);
		if(client < 0)
		{
		    shock_log(SHOCK_ERROR, "Couldn't accept incoming connection!");
		    printf("[Connection Error] Failed to accept incoming connection!");
			continue;
		}

        shock_create_thread(processRequest, client);
    }

    printf("--- END, THAT SHOULDN'T BE REACHED ---");
    return 0;
}
