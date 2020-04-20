#include "UDPPackager.h"
char *UDPPackageCreate(UDPPackageTypes type, void *data, size_t size)
{
    char *buffer;
    switch (type)
    {
    case UDPTypeText:
        buffer = (char *)SDL_malloc(size + 1);
        sprintf(buffer, "0%s", (char *)data);
        break;
    case UDPTypeEntity:
        buffer = (char *)SDL_malloc(sizeof(Entity) + 1);
        sprintf(buffer, "1%s", (char *)data);
        break;
    default:
        break;
    }
    return buffer;
}
UDPPackageTypes UDPPackageDecode(char *data)
{
    switch (data[0])
    {
    case '0':
        return UDPTypeText;
    case '1':
        return UDPTypeEntity;
    default:
        return 400;
    }
}
void UDPPackageDestroy(void *data)
{
    SDL_free(data);
}