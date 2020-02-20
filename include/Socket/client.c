#include "SocketWrapper.h"
//gcc client.c SocketWrapper.c -L/usr/lib/x86_64-linux-gnu -lSDL2_net -o client.out
//gcc server.c SocketWrapper.c -L/usr/lib/x86_64-linux-gnu -lSDL2_net -o server.out
int main(int argc, char **argv) 
{ 
    SocketClient client = SocketClientInit("127.0.0.1", 2000, SDL_FALSE);
    SocketClientMSG(client, "dood");
}