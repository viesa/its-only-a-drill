#include "UDPClient.h"

void UDPClientInitialize()
{
    udpClient.isInitialized = SDL_FALSE;
    udpClient.isActive = SDL_FALSE;
    udpClient.receivedPlayerID = SDL_FALSE;
    udpClient.inBuffer = VectorCreate(sizeof(ParsedUDPPacket), 100);
    udpClient.inBufferMutex = SDL_CreateMutex();

    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        log_error("Failed to init SDL_net: %s", SDLNet_GetError());
        return;
    }
    /* Open a socket on random port */
    if (!(udpClient.socket = SDLNet_UDP_Open(0)))
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
    if (SDLNet_ResolveHost(&udpClient.serverIP, ip, port) == -1)
    {
        log_error("Failed to resolve host: (%s %d): %s", ip, port, SDLNet_GetError());
        return;
    }

    udpClient.isInitialized = SDL_TRUE;
}

void UDPClientUninitialize()
{
    UDPClientSend(UDPType_Text, "quit", 5);
    for (size_t i = 0; i < udpClient.inBuffer->size; i++)
    {
        ParsedUDPPacketDestroy(&UDPCLIENT_INBUFFER[i]);
    }
    VectorDestroy(udpClient.inBuffer);
    SDL_UnlockMutex(udpClient.inBufferMutex);
    SDL_DestroyMutex(udpClient.inBufferMutex);
    SDLNet_UDP_Close(udpClient.socket);
    SDLNet_Quit();
    udpClient.isInitialized = SDL_FALSE;
}

void UDPClientStart()
{
    assert("Attempting to send UDP-packet without client initialization" && udpClient.isInitialized);

    udpClient.isActive = SDL_TRUE;
    udpClient.listenThread = SDL_CreateThread((SDL_ThreadFunction)UDPClientListenToServer, "Server Listen Thread", NULL);
    if (UDPClientSend(UDPType_Text, "alive", 6))
    {
#ifdef UDPCLIENT_DEBUG
        log_info("Sent message: \"alive\"");
#endif
    }
    else
    {
#ifdef UDPCLIENT_DEBUG
        log_error("Failed to send \"alive\" message to server");
#endif
    }
}

void UDPClientStop()
{
    udpClient.isActive = SDL_FALSE;
    SDL_WaitThread(udpClient.listenThread, NULL);
}

int UDPClientSend(UDPPacketType type, void *data, size_t size)
{
    assert("Attempting to send UDP-packet without client initialization" && udpClient.isInitialized);

    UDPpacket *outgoing = UDPPacketCreate(type, data, size);
    outgoing->address.host = udpClient.serverIP.host;
    outgoing->address.port = udpClient.serverIP.port;
#ifdef UDPCLIENT_DEBUG_RAWINOUT
    printf(" -- PACKET OUTGOING TO (%d | %d)\n", outgoing->address.host, outgoing->address.port);
    printf(" Type: %d Raw Message: ", type);
    for (int i = 1; i < outgoing->len; i++)
    {
        printf("%c", ((char *)outgoing->data)[i]);
    }
    printf("\n -- END OF PACKET\n");
#endif

    int result = SDLNet_UDP_Send(udpClient.socket, -1, outgoing);

    UDPPacketDestroy(outgoing);

    return result;
}

void UDPClientListenToServer()
{
    assert("Attempting to send UDP-packet without client initialization" && udpClient.isInitialized);

    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (udpClient.isActive)
    {
        UDPpacket *incoming = SDLNet_AllocPacket(MAX_MSGLEN);
        incoming->address.host = udpClient.serverIP.host;
        incoming->address.port = udpClient.serverIP.port;

        int result = SDLNet_UDP_Recv(udpClient.socket, incoming);
        if (result == -1)
        {
            log_warn("Failed to recieve UDP-packet: ", SDLNet_GetError());
        }
        if (result)
        {
            UDPPacketType type = UDPPacketDecode((char *)incoming->data);
            UDPPacketRemoveType(incoming);
            ParsedUDPPacket parsedPacket = ParsedUDPPacketCreate(type, incoming->data, incoming->len, incoming->address);
            SDL_LockMutex(udpClient.inBufferMutex);
            VectorPushBack(udpClient.inBuffer, &parsedPacket);
#ifdef UDPCLIENT_DEBUG_RAWINOUT
            printf(" -- PACKET INCOMING FROM (host | port) (%d | %d)\n", parsedPacket.sender.host, parsedPacket.sender.port);
            printf(" Type: %d Raw Message: ", parsedPacket.type);
            for (int i = 0; i < parsedPacket.size; i++)
            {
                printf("%c", ((char *)parsedPacket.data)[i]);
            }
            printf("\n -- END OF PACKET\n");
#endif
            SDL_UnlockMutex(udpClient.inBufferMutex);
        }
        else
        {
            SDL_Delay(0);
        }

        SDLNet_FreePacket(incoming);
    }
}

ParsedUDPPacket *UDPClientGetInBufferArray()
{
    return (ParsedUDPPacket *)udpClient.inBuffer->data;
}

size_t UDPClientGetInBufferArraySize()
{
    return udpClient.inBuffer->size;
}

Vector *UDPClientGetInBufferVector()
{
    return udpClient.inBuffer;
}

void UDPClientPlayerIsInitialized()
{
    udpClient.receivedPlayerID = SDL_TRUE;
}