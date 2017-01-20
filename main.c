#include <SDL.h>
#include <stdio.h>
//#include <SDL2/SDL_net.h>
#include "request.h"
#include "response.h"
#include "error.h"
#include "server_defines.h"
#include "config_file.h"

#include "base.h"

shock_config_t conf;

void processRequest(SOCKET sock) {
    char buffer[2048];
    //SDLNet_TCP_Recv(sock, &buffer, sizeof(buffer));
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

    //SDLNet_TCP_Close(sock);
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
    if(SDL_Init(0)==-1) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        printf("[Error] exiting...\n");
        printf("Press Enter to exit\n");
        getchar();
        return 1;
    }
    /*
    if(SDLNet_Init()==-1) {
        printf("SDLnet_Init error: %s\n", SDLNet_GetError());
        printf("[Error] exiting...\n");
        SDL_Delay(ERROR_DELAY);
        return 1;
    }*/

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
        //SDL_Delay(ERROR_DELAY);
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

    /*if(SDLNet_ResolveHost(&ip, NULL, conf.port) == -1) {
        printf("SDLnet_ResolveHost error: %s\n", SDLNet_GetError());
        printf("[Error] exiting...\n");
        SDL_Delay(ERROR_DELAY);
        return 1;
    }*/

    printf("Starting server...\n\n");
    /*TCPsocket server = SDLNet_TCP_Open(&ip);
    if (server == NULL) {
        printf("SDLnet_TCP_Open error: %s\n", SDLNet_GetError());
        printf("[Error] exiting...\n");
        SDL_Delay(ERROR_DELAY);
        return 1;
    }*/

    if (listen(server, 5) == SOCKET_ERROR) {
        printf("[Fatal Error] Cannot start listening on given port! \n");
        return 1;
    }


    //TCPsocket client;
    SOCKET client;
    struct sockaddr_in clientIP;
    size_t c = sizeof(struct sockaddr_in);

    printf("[!] Done. Waiting for incoming connections...\n");
    while (1) {
        //client=SDLNet_TCP_Accept(server);
        client=accept(server, (struct sockaddr *)&clientIP, (socklen_t*)&c);
		if(client < 0)
		{
		    printf("[Connection Error] Failed to accept incoming connection!");
			continue;
		}
		SDL_Thread* thread;
        thread = SDL_CreateThread(processRequest, "ClientProcessThread", client);

        if (thread == NULL) {
            printf("[Error] Failed to create SDL_Thread for new connection. Making sync call...");
            processRequest(client);
        }
    }

    //printf("CLEANUP: cleaning SDLnet up\n");
    //SDLNet_Quit();
    printf("CLEANUP: cleaning SDL up\n");
    SDL_Quit();
    printf("--- END ---");
    return 0;
}
