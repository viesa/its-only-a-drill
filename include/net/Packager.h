#ifndef UDPPACKAGER_H
#define UDPPACKAGER_H

#include "../core/Dependencies.h"
#include "NetPlayer.h"

#define MAX_MSGLEN 2048

#define TCP_MAX_SIZE 65535 - 2000
#define TCP_HEADER_SIZE 10

#define NET_TYPE_SIZE 3
#define NET_ID_SIZE 3

typedef enum PacketType
{
    PT_Text,             // When plain text is to be sent
    PT_Connect,          // When client connects to server
    PT_Disconnect,       // When client or server disconnectes
    PT_NewPlayer,        // When server broadcasts a new player join,
                         // or when server updates a new client with current players
    PT_DelPlayer,        // When server broadcasts deletion of a player
    PT_Entity,           // When sending an entity
    PT_CompressedEntity, // When sending a compressed entity
    PT_CreateSession,    // When client wants to create a lobby
    PT_JoinSession,      // When client wants to join a lobby
    PT_LeaveSession,
    PT_None,
    PT_Count
} PacketType;

typedef struct ParsedPacket
{
    PacketType type;
    void *data;
    size_t size;
    NetPlayer sender;
} ParsedPacket;

// An "extension" to SDL, since the don't have TCPpacket as a datatype
typedef struct TCPpacket
{
    Uint8 *data;
    int len;
    TCPsocket address;
} TCPpacket;

ParsedPacket ParsedPacketCreate(PacketType type, void *data, size_t size, NetPlayer sender);
void ParsedPacketDestroy(ParsedPacket *packet);

// Allocates a UDP-packet
UDPpacket *UDPPacketCreate(PacketType type, int id, void *data, size_t size);
// Frees the UDP-packet
void UDPPacketDestroy(UDPpacket *packet);
// Removes the UDP-packet type and leaves only the data
void UDPPacketRemoveTypeAndID(UDPpacket *packet);
// Allocates a TCP-packet
TCPpacket *TCPPacketCreate(PacketType type, int id, void *data, size_t size);
// Frees the TCP-packet
void TCPPacketDestroy(TCPpacket *packet);
/// Removes the TCP-packet type and leaves only the data
/// \return: New size of data
int TCPPacketRemoveTypeAndID(void *data, int len);
// Returns the type of a packet from UNFORMATTED data
PacketType PacketDecodeType(void *data);
// Returns the ID of a packet from UNFORMATTED data
PacketType PacketDecodeID(void *data);
/// Prints information about a TCP/UDP packet that is outgoing or incoming
/// ONLY IN DEBUG
/// \param protocol: "UDP" or "TCP"
/// \param inout: "OUTGOING" or "INCOMING"
void PacketPrintInformation(PacketType type, int id, void *data, int size, IPaddress ip, const char *protocol, const char *inout);

#endif