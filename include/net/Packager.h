#ifndef UDPPACKAGER_H
#define UDPPACKAGER_H

#include "../core/Dependencies.h"

#define MAX_MSGLEN 2048

typedef enum PacketType
{
    PT_Text,
    PT_PlayerID,
    PT_Entity,
    PT_CompressedEntity,
    PT_IPaddress,
    PT_None,
    PT_Count
} PacketType;

typedef struct ParsedPacket
{
    PacketType type;
    void *data;
    size_t size;
    IPaddress sender;
} ParsedPacket;

ParsedPacket ParsedPacketCreate(PacketType type, void *data, size_t size, IPaddress sender);
void ParsedPacketDestroy(ParsedPacket *packet);

// Allocates a package like this "{type}{data}\0"
UDPpacket *UDPPacketCreate(PacketType type, void *data, size_t size);
// SDL_free the data
void UDPPacketDestroy(UDPpacket *packet);
// Returns the type of a package from data
PacketType UDPPacketDecode(void *data);
// Removes the type in "{type}{data}\0" (usefull when dealing with strings)
void UDPPacketRemoveType(UDPpacket *packet);

#endif