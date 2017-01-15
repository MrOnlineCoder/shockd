#include <SDL.h>
#include <stdio.h>
#include <SDL2/SDL_net.h>
#include "request.h"
#include "response.h"
#include "error.h"
#include "server_defines.h"

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
    sprintf(filename, "%s%s", HTDOCS_FOLDER, route);

    shock_serve_file(sock, filename);

    SDLNet_TCP_Close(sock);
}

int main(int argc, char* argv[]) {
    printf("shockd server by MrOnlineCoder\n");
    printf("INIT: calling SDL_Init()\n");
    if(SDL_Init(0)==-1) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        printf("ERROR: exiting...\n");
        return 1;
    }
    printf("INIT: calling SDLnet_Init()\n");
    if(SDLNet_Init()==-1) {
        printf("SDLnet_Init error: %s\n", SDLNet_GetError());
        printf("ERROR: exiting...\n");
        return 1;
    }

    IPaddress ip;

    printf("INIT: resolving localhost:3000 address..\n");
    if(SDLNet_ResolveHost(&ip, NULL, 3000) == -1) {
        printf("SDLnet_ResolveHost error: %s\n", SDLNet_GetError());
        printf("ERROR: exiting...\n");
        return 1;
    }

    printf("INIT: starting server....\n");
    TCPsocket server = SDLNet_TCP_Open(&ip);
    if (server == NULL) {
        printf("SDLnet_TCP_Open error: %s\n", SDLNet_GetError());
        printf("ERROR: exiting...\n");
        return 1;
    }
    TCPsocket client;

    printf("SERVER: Loading complete! Now waiting for incoming connections...\n");
    while (1) {
        client=SDLNet_TCP_Accept(server);
		if(!client)
		{
			SDL_Delay(100);
			continue;
		}
        processRequest(client);
    }

    printf("CLEANUP: cleaning SDLnet up\n");
    SDLNet_Quit();
    printf("CLEANUP: cleaning SDL up\n");
    SDL_Quit();
    printf("--- END ---");
    return 0;
}
