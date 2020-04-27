#include "Client.h"

void ClientInitialize()
{
    client.isInitialized = SDL_FALSE;
    client.isActive = SDL_FALSE;
    client.receivedPlayerID = SDL_FALSE;
    client.inBuffer = VectorCreate(sizeof(ParsedPacket), 100);
    client.inBufferMutex = SDL_CreateMutex();

    /* Open a socket on random port */
    if (!(client.socket = SDLNet_UDP_Open(0)))
    {
        log_error("Failed to open socket on random port: %s", SDLNet_GetError());
        return;
    }
/* Resolve server name  */
#ifdef UDPSERVER_LOCAL
    const char *ip = "127.0.0.1";
    Uint16 port = 1337;
#else
    const char *ip = "85.226.160.126"; //drill.pierrelf.com port 1337
    Uint16 port = 1337;
#endif
    if (SDLNet_ResolveHost(&client.serverIP, ip, port) == -1)
    {
        log_error("Failed to resolve host: (%s %d): %s", ip, port, SDLNet_GetError());
        return;
    }

    client.isInitialized = SDL_TRUE;
}

void ClientUninitialize()
{
    ClientSend(PT_Text, "quit", 5);
    for (size_t i = 0; i < client.inBuffer->size; i++)
    {
        ParsedPacketDestroy(&CLIENT_INBUFFER[i]);
    }
    VectorDestroy(client.inBuffer);
    SDL_UnlockMutex(client.inBufferMutex);
    SDL_DestroyMutex(client.inBufferMutex);
    SDLNet_UDP_Close(client.socket);
    client.isInitialized = SDL_FALSE;
}

void ClientStart()
{
    assert("Attempting to send UDP-packet without client initialization" && client.isInitialized);

    client.isActive = SDL_TRUE;
    client.listenThread = SDL_CreateThread((SDL_ThreadFunction)ClientListenToServer, "Server Listen Thread", NULL);
    if (ClientSend(PT_Text, "alive", 6))
    {
#ifdef CLIENT_DEBUG
        log_info("Sent message: \"alive\"");
#endif
    }
    else
    {
#ifdef CLIENT_DEBUG
        log_error("Failed to send \"alive\" message to server");
#endif
    }
}

void ClientStop()
{
    client.isActive = SDL_FALSE;
    SDL_WaitThread(client.listenThread, NULL);
}

int ClientSend(PacketType type, void *data, size_t size)
{
    assert("Attempting to send UDP-packet without client initialization" && client.isInitialized);

    UDPpacket *outgoing = UDPPacketCreate(type, data, size);
    outgoing->address.host = client.serverIP.host;
    outgoing->address.port = client.serverIP.port;
#ifdef CLIENT_DEBUG_RAWINOUT
    printf(" -- PACKET OUTGOING TO (%d | %d)\n", outgoing->address.host, outgoing->address.port);
    printf(" Type: %d Raw Message: ", type);
    for (int i = 1; i < outgoing->len; i++)
    {
        printf("%c", ((char *)outgoing->data)[i]);
    }
    printf("\n -- END OF PACKET\n");
#endif

    int result = SDLNet_UDP_Send(client.socket, -1, outgoing);

    UDPPacketDestroy(outgoing);

    return result;
}

void ClientListenToServer()
{
    assert("Attempting to send UDP-packet without client initialization" && client.isInitialized);

    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (client.isActive)
    {
        UDPpacket *incoming = SDLNet_AllocPacket(MAX_MSGLEN);
        incoming->address.host = client.serverIP.host;
        incoming->address.port = client.serverIP.port;

        int result = SDLNet_UDP_Recv(client.socket, incoming);
        if (result == -1)
        {
            log_warn("Failed to recieve UDP-packet: ", SDLNet_GetError());
        }
        if (result)
        {
            PacketType type = UDPPacketDecode((char *)incoming->data);
            UDPPacketRemoveType(incoming);
            ParsedPacket parsedPacket = ParsedPacketCreate(type, incoming->data, incoming->len, incoming->address);
            SDL_LockMutex(client.inBufferMutex);
            VectorPushBack(client.inBuffer, &parsedPacket);
#ifdef CLIENT_DEBUG_RAWINOUT
            printf(" -- PACKET INCOMING FROM (host | port) (%d | %d)\n", parsedPacket.sender.host, parsedPacket.sender.port);
            printf(" Type: %d Raw Message: ", parsedPacket.type);
            for (int i = 0; i < parsedPacket.size; i++)
            {
                printf("%c", ((char *)parsedPacket.data)[i]);
            }
            printf("\n -- END OF PACKET\n");
#endif
            SDL_UnlockMutex(client.inBufferMutex);
        }
        else
        {
            SDL_Delay(0);
        }

        SDLNet_FreePacket(incoming);
    }
}

ParsedPacket *ClientGetInBufferArray()
{
    return (ParsedPacket *)client.inBuffer->data;
}

size_t ClientGetInBufferArraySize()
{
    return client.inBuffer->size;
}

Vector *ClientGetInBufferVector()
{
    return client.inBuffer;
}

void ClientPlayerIsInitialized()
{
    client.receivedPlayerID = SDL_TRUE;
}