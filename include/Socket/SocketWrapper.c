#include "SocketWrapper.h"
SocketClient SocketClientInit(char host[], int port, SDL_bool isServer){
    SocketClient client;
    if (SDLNet_Init() < 0)
    {   
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if (isServer){
        if (SDLNet_ResolveHost(&client.ip, NULL, port) < 0) 
        { 
            fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError()); 
            exit(EXIT_FAILURE); 
        }
    }
    else{
        if (SDLNet_ResolveHost(&client.ip, host, port) < 0) 
        { 
            fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError()); 
            exit(EXIT_FAILURE); 
        }
    }

    /* Open a connection with the IP provided (listen on the host's port) */ 
    if (SocketClientOpenPort(&client) != 0)
        return client;
    else
        printf("Port Crash");
    return client;
}
int SocketClientOpenPort(SocketClient *client){
    int i = 0;
    for(i = 1000; i < 10000; i++){
        if ((client->sock = SDLNet_TCP_Open(&client->ip)) > 0) 
        { 
            return i;
        }
    }
}
int SocketClientMSG(SocketClient client, char msg[]){
    /* Send messages */ 
    int quit, len; 
    quit = 0; 
    while (!quit) 
    {
        len = strlen(msg) + 1; 
        if (SDLNet_TCP_Send(client.sock, (void *)msg, len) < len) 
        { 
            fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError()); 
            return EXIT_FAILURE; 
        }
    }
}
int SocketClientListen(SocketClient server, char buffer[]){
    /* This check the server if there is a pending connection. * If there is one, accept that, and open a new socket for communicating */ 
    TCPsocket cSocket;   //clientSocket
    IPaddress cIp;      //clientIp
    if ((cSocket = SDLNet_TCP_Accept(server.sock))) 
    { 
        /* Now we can communicate with the client using client socket * sd will remain opened waiting other connections */
        /* Get the remote address */ 
        if ((&cIp == SDLNet_TCP_GetPeerAddress(cSocket))) /* Print the address, converting in the host format */ 
            printf("Host connected: %x %d\n", SDLNet_Read32(&cIp.host), SDLNet_Read16(&cIp.port));
        else 
            fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
        if (SDLNet_TCP_Recv(cSocket, buffer, BUFFER_LENGTH) > 0) 
        { 
            //success
        }
        /* Close the client socket */ 
        SDLNet_TCP_Close(cSocket);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}