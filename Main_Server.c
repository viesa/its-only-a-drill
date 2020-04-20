#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "include/core/Event.h"
#include "include/net_UDP/UDPServer.h"
//#define DEGBUG
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
            char buffer[MAX_MSGLEN];
            sprintf(buffer, "%s (host:port) %x:%x", m_server.pack->data, m_server.pack->address.host, m_server.pack->address.port);
            UDPServerEcho(&m_server, UDPTypeText, buffer, strlen(buffer) + 1);
        }
        if (rLen && UDPPackageDecode((char *)m_server.pack->data) == UDPTypeEntity)
        {
            UDPPackageRemoveTypeNULL(m_server.pack);
            UDPServerEcho(&m_server, UDPTypeEntity, (char *)m_server.pack->data, sizeof(Entity));
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