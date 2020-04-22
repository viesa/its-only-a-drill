#ifndef UDPPACKAGER_H
#define UDPPACKAGER_H
#include "SDL2/SDL_net.h"
// only for packaging purposes
#include "../core/Entity.h"
typedef enum UDPPackageTypes
{
    UDPTypeText,
    UDPTypeint,
    UDPTypeEntity
} UDPPackageTypes;
// Allocates a package like this "{type}{data}\0"
char *UDPPackageCreate(UDPPackageTypes type, void *data, size_t size);
// Returns the type of a package from data
UDPPackageTypes UDPPackageDecode(char *data);
// Removes the type in "{type}{data}\0" (usefull when dealing with strings)
void UDPPackageRemoveType(UDPpacket *pack);
// Removes the type and NULL terminator in "{type}{data}\0" (usefull when dealing with data structures)
void UDPPackageRemoveTypeNULL(UDPpacket *pack);
// SDL_free the data
void UDPPackageDestroy(void *data);
#endif