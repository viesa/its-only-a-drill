#include "UDPPackager.h"

ParsedUDPPacket ParsedUDPPacketCreate(UDPPacketType type, void *data, size_t size, IPaddress sender)
{
    ParsedUDPPacket packet;
    packet.type = type;
    packet.data = (void *)MALLOC_N(char, size);
    ALLOC_ERROR_CHECK(packet.data);
    memcpy(packet.data, data, size);
    packet.size = size;
    packet.sender = sender;
    return packet;
}
void ParsedUDPPacketDestroy(ParsedUDPPacket *packet)
{
    if (packet)
        SDL_free(packet->data);
}

UDPpacket *UDPPacketCreate(UDPPacketType type, void *data, size_t size)
{
    UDPpacket *packet = SDLNet_AllocPacket(size + 1);

    char *itoa_buffer = MALLOC(char);
    SDL_itoa((int)type, itoa_buffer, 10);
    SDL_memcpy(packet->data, itoa_buffer, 1);
    SDL_memcpy(packet->data + 1, data, size);
    SDL_free(itoa_buffer);
    packet->len = size + 1;

    return packet;
}

void UDPPacketDestroy(UDPpacket *packet)
{
    if (packet)
        SDLNet_FreePacket(packet);
}

UDPPacketType UDPPacketDecode(void *data)
{
    switch (((char *)data)[0])
    {
    case '0':
        return UDPType_Text;
    case '1':
        return UDPType_PlayerID;
    case '2':
        return UDPType_Entity;
    case '3':
        return UDPType_CompressedEntity;
    case '4':
        return UDPType_IPaddress;
    default:
        return 400;
    }
}
void UDPPacketRemoveType(UDPpacket *packet)
{
    SDL_memmove(packet->data, packet->data + 1, packet->len - 1);
    packet->len -= 1;
    SDLNet_ResizePacket(packet, -1);
}
