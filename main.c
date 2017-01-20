#include <SDL.h>
#include <stdio.h>
#include <SDL2/SDL_net.h>
#include "request.h"
#include "response.h"
#include "error.h"
#include "server_defines.h"
#include "config_file.h"

shock_config_t conf;

void processRequest(TCPsocket sock) {
    char buffer[2048];
    SDLNet_TCP_Recv(sock, &buffer, sizeof(buffer));
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

    SDLNet_TCP_Close(sock);
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
    if(SDLNet_Init()==-1) {
        printf("SDLnet_Init error: %s\n", SDLNet_GetError());
        printf("[Error] exiting...\n");
        SDL_Delay(ERROR_DELAY);
        return 1;
    }


    IPaddress ip;

    printf("Applying configuration from shockd.conf file...\n\n");
    shock_config_t config;
    shock_default_config(&config);
    if (shock_parse_config(&config, "shockd.conf") == -1) {
        printf("FATAL ERROR: cannot parse config!");
        SDL_Delay(ERROR_DELAY);
        return 1;
    }
    conf = config;
    printf("[Config] Port: %d\n", conf.port);
    printf("[Config] Server Root: %s\n", conf.root);
    printf("[Config] ClearLogs: %d\n", conf.clearLogs);


    printf("\nResolving IP...\n\n");
    if(SDLNet_ResolveHost(&ip, NULL, conf.port) == -1) {
        printf("SDLnet_ResolveHost error: %s\n", SDLNet_GetError());
        printf("[Error] exiting...\n");
        SDL_Delay(ERROR_DELAY);
        return 1;
    }

    printf("Starting server...\n\n");
    TCPsocket server = SDLNet_TCP_Open(&ip);
    if (server == NULL) {
        printf("SDLnet_TCP_Open error: %s\n", SDLNet_GetError());
        printf("[Error] exiting...\n");
        SDL_Delay(ERROR_DELAY);
        return 1;
    }


    TCPsocket client;

    printf("[!] Done. Waiting for incoming connections...\n");
    while (1) {
        client=SDLNet_TCP_Accept(server);
		if(!client)
		{
		    SDL_Delay(10);
			continue;
		}
		SDL_Thread* thread;
        thread = SDL_CreateThread(processRequest, "ClientProcessThread", client);

        if (thread == NULL) {
            printf("[Error] Failed to create SDL_Thread for new connection. Making sync call...");
            processRequest(client);
        }
    }

    printf("CLEANUP: cleaning SDLnet up\n");
    SDLNet_Quit();
    printf("CLEANUP: cleaning SDL up\n");
    SDL_Quit();
    printf("--- END ---");
    return 0;
}
