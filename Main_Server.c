#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "include/core/Event.h"
#include "include/net_UDP/UDPServer.h"
int main()
{
    SDL_bool m_running = SDL_TRUE;
    UDPServer m_server = UDPServerCreate(2000);

    while (m_running)
    {
        int rLen = UDPServerListen(&m_server, MAX_MSGLEN);
        if (rLen)
        {
            char buffer[MAX_MSGLEN];
            sprintf(buffer, "%s (host:port) %x:%x", m_server.pack->data, m_server.pack->address.host, m_server.pack->address.port);
            UDPServerBroadcast(&m_server, buffer, strlen(buffer) + 1);
        }
    }
    UDPServerDestroy(&m_server);
    SDL_Quit();

    return EXIT_SUCCESS;
}