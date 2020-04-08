#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "include/core/Event.h"
#include "include/net_UDP/UDPServer.h"

int main()
{
    SDL_bool m_running = SDL_TRUE;
    UDPServer m_server = UDPServerCreate(1337);

    while (m_running)
    {
        SDL_Delay(100);
        int rLen = UDPServerListen(&m_server, 100);
        if (rLen)
        {
            int ports[1] = {m_server.pack->address.port};
            printf("Message Recieved: %s\n", m_server.pack->data);
            UDPServerSend(&m_server,
                          m_server.pack->data,
                          m_server.pack->len,
                          m_server.pack->address.port);
        }
    }
    UDPServerDestroy(&m_server);
    SDL_Quit();

    return EXIT_SUCCESS;
}