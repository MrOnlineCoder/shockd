#include <stdio.h>
#include "request.h"
#include "response.h"
#include "error.h"
#include "server_defines.h"
#include "config_file.h"
#include "thread_wrapper.h"

#include "base.h"

shock_config_t conf;

void processRequest(SOCKET sock) {
    char buffer[2048];
    recv(sock, &buffer, sizeof(buffer), 0);
    shock_request_t request;
    //Parse and validate request
    int reqError = shock_parse_request(&request, &buffer, strlen(buffer));
    if (reqError == -1) {
        shock_error_badrequest(sock, &request);
        return;
    }

    //Now serve the file!
    char* route = request.route;
    char lastchar = route[strlen(route)-1];
    if (lastchar == '/') {
        strcat(route, "index.html");
    }
    char filename[FILENAME_MAX] = "";
    sprintf(filename, "%s%s", conf.root, route);

    shock_serve_file(sock, filename);

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


    printf("\nResolving IP...\n\n");

    SOCKET server;

    struct sockaddr_in serverIP;
    server = socket(AF_INET , SOCK_STREAM , 0);
    if (server == -1)
    {
       printf("[Fatal Error] Cannot create server socket!");
       return 1;
    }

    //Prepare the sockaddr_in structure
    serverIP.sin_family = AF_INET;
    serverIP.sin_addr.s_addr = INADDR_ANY;
    serverIP.sin_port = htons( conf.port );

    if( bind(server,(struct sockaddr *)&serverIP , sizeof(serverIP)) < 0)
    {
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

    printf("[!] Done. Waiting for incoming connections...\n");
    while (1) {
        client=accept(server, (struct sockaddr *)&clientIP, (socklen_t*)&c);
		if(client < 0)
		{
		    printf("[Connection Error] Failed to accept incoming connection!");
			continue;
		}

        shock_create_thread(processRequest, client);
    }

    printf("--- END ---");
    return 0;
}
