#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "include/core/Event.h"
#include "include/net_UDP/UDPServer.h"

#include "include/core/AppServer.h"

int main()
{
    SDL_bool m_running = SDL_TRUE;
    UDPServer m_server = UDPServerCreate(4000);

    while (m_running)
    {
        int rLen = UDPServerListen(&m_server, 100);
        if (rLen)
        {
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