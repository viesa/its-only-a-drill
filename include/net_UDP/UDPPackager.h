#ifndef UDPPACKAGER_H
#define UDPPACKAGER_H
// only for packaging purposes
#include "../core/Entity.h"
typedef enum UDPPackageTypes
{
    UDPTypeText,
    UDPTypeEntity
} UDPPackageTypes;
char *UDPPackageCreate(UDPPackageTypes type, void *data, size_t size);
UDPPackageTypes UDPPackageDecode(char *data);
void UDPPackageDestroy(void *data);
#endif