#ifndef UDPPACKAGER_H
#define UDPPACKAGER_H

#include "Dependencies.h"
#include "NetPlayer.h"

#define MAX_MSGLEN 2048

#define TCP_MAX_SIZE 63535
#define TCP_HEADER_SIZE 10

#define NET_TYPE_SIZE 3
#define NET_ID_SIZE 3

typedef enum PacketType
{
    PT_Text,             // When plain text is to be sent
    PT_AreYouAlive,      // When client or server want to check if the other one is still replying/connected
    PT_IAmAlive,         // When the client or server want to confirm that he is is replying/connected
    PT_IsPlayerActive,   // When the client wants to check if players in sessions are still there
    PT_Connect,          // When client connects to server, recieve their name sends out ID
    PT_UDPRespondIP,     // When client want to notify server which IP-address to asssign it's outgoing UDP-packets with
    PT_DuplicateName,    // When client has a duplicate name
    PT_Disconnect,       // When client or server disconnectes
    PT_NewPlayer,        // When server broadcasts a new player join,
                         // or when server updates a new client with current players
    PT_DelPlayer,        // When server broadcasts deletion of a player
    PT_Entity,           // When sending an entity
    PT_CompressedEntity, // When sending a compressed entity
    PT_CloseSession,     // When server notifies clients that session is closing
    PT_CreateSession,    // When client wants to create a lobby
    PT_JoinSession,      // When client wants to join a lobby
    PT_LeaveSession,     // When client notifies the server it is leaving the session
    PT_FullSession,      // When the server notifies the client the session is full
    PT_ChangeSkin,       // When the client changes skins in the lobby
    PT_HostAssign,       // When the host notfies the ID of the host
    PT_FetchSessions,    // When client asks server what sessions exists
    PT_FetchLobby,       // When client asks server which players are in given lobby
    PT_StartRound,       // When host starts the session from the lobby, server returns the entity of the player
    PT_PlayerHit,        // When a player is getting shot [0] = id of shot player, [1] = damage
    PT_PlayerShoot,      // When a player shoots this is sent so other clients can draw the line
    PT_PlayerDead,       // When a player is killed, it notifies the server with this packet so other players will know too
    PT_NewRound,         // When everyone died in a round, start a new round
    PT_CloseAllSessions, // When server want to close all running sessions
    PT_MatchFinished,    // When server notifying the clients the match is over
    PT_RoundFinished,    // When server notifying the clients the round is over
    PT_Countdown,        // When server updates client with roundFinished-countdown or matchedFinished-countdown
    PT_FetchScoreboard,  // When client asks server about everyones score
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