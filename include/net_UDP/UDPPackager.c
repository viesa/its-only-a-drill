#include "UDPPackager.h"
char *UDPPackageCreate(UDPPackageTypes type, void *data, size_t size)
{
    char *buffer = (char *)SDL_malloc(size + 2);
    switch (type)
    {
    case UDPTypeText:
        buffer++;
        SDL_memcpy(buffer, data, size);
        buffer--;
        buffer[0] = '0';
        buffer[size + 1] = '\0';
        break;
    case UDPTypeEntity:
        buffer++;
        SDL_memcpy(buffer, data, size);
        buffer--;
        buffer[0] = '1';
        buffer[size + 1] = '\0';
        break;
    default:
        break;
    }
    // for (int i = 0; i < size + 1; i++)
    // {
    //     printf("%c", buffer[i]);
    // }
    // printf("\n");
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
void UDPPackageRemoveTypeNULL(UDPpacket *pack)
{
    for (int i = 1; i < pack->len - 1; i++)
    {
        pack->data[i - 1] = pack->data[i];
    }
    SDLNet_ResizePacket(pack, -2);
}
void UDPPackageRemoveType(UDPpacket *pack)
{
    for (int i = 1; i < strlen((char *)pack->data) + 1; i++)
    {
        pack->data[i - 1] = pack->data[i];
    }
    SDLNet_ResizePacket(pack, -1);
}
void UDPPackageDestroy(char *data)
{
    SDL_free(data);
}