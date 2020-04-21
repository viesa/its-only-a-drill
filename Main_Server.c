#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "include/core/Event.h"
#include "include/net_UDP/UDPServer.h"
#define DEGBUG
int main()
{
    SDL_bool m_running = SDL_TRUE;
    UDPServer m_server = UDPServerCreate(1337);
    while (m_running)
    {
        int rLen = UDPServerListen(&m_server, MAX_MSGLEN);
        if (rLen && UDPPackageDecode((char *)m_server.pack->data) == UDPTypeText)
        {
            UDPPackageRemoveType(m_server.pack);
            if (!strcmp((char *)m_server.pack->data, "alive"))
            {
                for (int i = 0; i < m_server.nrPlayers; i++)
                {
                    if (m_server.ids[i] == 0)
                    {
                        m_server.ids[i] = i + 1;
                        m_server.players[i].id = i + 1;
                        UDPServerSend(&m_server, UDPTypeint, &m_server.players[i].id, sizeof(int), m_server.pack->address);
                    }
                }
            }
        }
        if (rLen && UDPPackageDecode((char *)m_server.pack->data) == UDPTypeEntity)
        {
            UDPPackageRemoveTypeNULL(m_server.pack);
            char buffer[m_server.pack->len - 2];
            SDL_memcpy(buffer, m_server.pack->data, m_server.pack->len - 2);
            UDPServerEcho(&m_server, UDPTypeEntity, buffer, sizeof(Entity));
            log_info("Recieved entity by %x:%x\nBroadcasting...\n", m_server.pack->address.host, m_server.pack->address.port);
        }

#ifdef DEGBUG
        if (rLen && UDPPackageDecode((char *)m_server.pack->data) == UDPTypeEntity)
        {
            UDPPackageRemoveTypeNULL(m_server.pack);
            UDPServerEcho(&m_server, UDPTypeEntity, (char *)m_server.pack->data, sizeof(Entity));
            Entity *ent = (Entity *)m_server.pack->data;
            printf("\n");
            printf("(%x:%x) coordinates:\nx:%f, y:%f\n", m_server.pack->address.host, m_server.pack->address.port, ent->position.x, ent->position.y);
        }
#endif
    }
    UDPServerDestroy(&m_server);
    SDL_Quit();

    return EXIT_SUCCESS;
}