#include "Packager.h"

ParsedPacket ParsedPacketCreate(PacketType type, void *data, size_t size, NetPlayer sender)
{
    ParsedPacket packet;
    packet.type = type;
    packet.data = (void *)MALLOC_N(char, size);
    ALLOC_ERROR_CHECK(packet.data);
    memcpy(packet.data, data, size);
    packet.size = size;
    packet.sender = sender;
    return packet;
}
void ParsedPacketDestroy(ParsedPacket *packet)
{
    if (packet)
        SDL_free(packet->data);
}

UDPpacket *UDPPacketCreate(PacketType type, int id, void *data, size_t size)
{
    const int totalLength = NET_TYPE_SIZE + NET_ID_SIZE + size;
    UDPpacket *packet = SDLNet_AllocPacket(totalLength);

    char itoa_typeBuffer[NET_TYPE_SIZE] = {0};
    char itoa_idBuffer[NET_ID_SIZE] = {0};
    SDL_itoa((int)type, itoa_typeBuffer, 10);
    SDL_itoa((int)id, itoa_idBuffer, 10);
    SDL_memcpy(packet->data, itoa_typeBuffer, NET_TYPE_SIZE);
    SDL_memcpy(packet->data + NET_TYPE_SIZE, itoa_idBuffer, NET_ID_SIZE);
    SDL_memcpy(packet->data + NET_TYPE_SIZE + NET_ID_SIZE, data, size);
    packet->len = totalLength;

    return packet;
}

void UDPPacketDestroy(UDPpacket *packet)
{
    if (packet)
        SDLNet_FreePacket(packet);
}

void UDPPacketRemoveTypeAndID(UDPpacket *packet)
{
    const int toRemove = NET_TYPE_SIZE + NET_ID_SIZE;
    SDL_memmove(packet->data, packet->data + toRemove, packet->len - toRemove);
    packet->len -= toRemove;
    SDL_memset(packet->data + packet->len, 0, toRemove);
}

TCPpacket *TCPPacketCreate(PacketType type, int id, void *data, size_t size)
{
    const int headerSize = TCP_HEADER_SIZE;
    const int packetSize = NET_TYPE_SIZE + NET_ID_SIZE + size;
    const int totalSize = headerSize + packetSize;

    TCPpacket *packet = MALLOC(TCPpacket);
    packet->len = totalSize;
    packet->data = MALLOC_N(Uint8, packet->len);
    ALLOC_ERROR_CHECK(packet->data);

    char itoa_headerBuffer[TCP_HEADER_SIZE] = {0};
    char itoa_typeBuffer[NET_TYPE_SIZE] = {0};
    char itoa_idBuffer[NET_ID_SIZE] = {0};
    SDL_itoa(packetSize, itoa_headerBuffer, 10);
    SDL_itoa((int)type, itoa_typeBuffer, 10);
    SDL_itoa(id, itoa_idBuffer, 10);
    SDL_memcpy(packet->data, itoa_headerBuffer, TCP_HEADER_SIZE);
    SDL_memcpy(packet->data + TCP_HEADER_SIZE, itoa_typeBuffer, NET_TYPE_SIZE);
    SDL_memcpy(packet->data + TCP_HEADER_SIZE + NET_TYPE_SIZE, itoa_idBuffer, NET_ID_SIZE);
    SDL_memcpy(packet->data + TCP_HEADER_SIZE + NET_TYPE_SIZE + NET_ID_SIZE, data, size);

    return packet;
}

void TCPPacketDestroy(TCPpacket *packet)
{
    if (packet)
        FREE(packet->data);
    FREE(packet);
}

int TCPPacketRemoveTypeAndID(void *data, int len)
{
    const int toRemove = NET_TYPE_SIZE + NET_ID_SIZE;
    SDL_memmove(data, data + toRemove, len - toRemove);
    return len - toRemove;
}

PacketType PacketDecodeType(void *data)
{
    char typeBuffer[NET_TYPE_SIZE + 1] = {0};
    SDL_memcpy(typeBuffer, data, NET_TYPE_SIZE);
    return (PacketType)SDL_atoi(typeBuffer);
}

PacketType PacketDecodeID(void *data)
{
    char idBuffer[NET_ID_SIZE + 1] = {0};
    SDL_memcpy(idBuffer, data + NET_TYPE_SIZE, NET_ID_SIZE);
    return SDL_atoi(idBuffer);
}

void PacketPrintInformation(PacketType type, int id, void *data, int size, IPaddress ip, const char *protocol, const char *inout)
{

    printf(" [%s] [%s] IP: %d:%d\n", protocol, inout, ip.host, ip.port);
    printf(" [ID:%d] [Type:%d] [Size:%d] \n", id, type, size);

#ifdef PACKAGER_DEBUG_RAWINOUT
    printf("\n ---- RAW DATA ----\n");
    for (int i = 0; i < size; i++)
    {
        printf("%c", ((char *)data)[i]);
    }
    printf("\n ------------------\n");
    printf("\n [%s] [%s] END OF PACKET\n", protocol, inout);
#endif
}