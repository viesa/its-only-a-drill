#ifndef UDPPACKAGER_H
#define UDPPACKAGER_H

#include "../core/Dependencies.h"

#define MAX_MSGLEN 2048

typedef enum UDPPacketType
{
    UDPType_Text,
    UDPType_PlayerID,
    UDPType_Entity,
    UDPType_CompressedEntity,
    UDPType_IPaddress,
    UDPType_None,
    UDPType_Count
} UDPPacketType;

typedef struct ParsedUDPPacket
{
    UDPPacketType type;
    void *data;
    size_t size;
    IPaddress sender;
} ParsedUDPPacket;

ParsedUDPPacket ParsedUDPPacketCreate(UDPPacketType type, void *data, size_t size, IPaddress sender);
void ParsedUDPPacketDestroy(ParsedUDPPacket *packet);

// Allocates a package like this "{type}{data}\0"
UDPpacket *UDPPacketCreate(UDPPacketType type, void *data, size_t size);
// SDL_free the data
void UDPPacketDestroy(UDPpacket *packet);
// Returns the type of a package from data
UDPPacketType UDPPacketDecode(void *data);
// Removes the type in "{type}{data}\0" (usefull when dealing with strings)
void UDPPacketRemoveType(UDPpacket *packet);

#endif