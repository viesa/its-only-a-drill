#ifndef FILELOADER_H
#define FILELOADER_H

#include "Dependencies.h"

typedef struct FileIO
{
    char *path;
    void *contents;
    int size;
} FileIO;

FileIO FileIOCreate(char *filename);
SDL_bool FileIOWrite(FileIO *file);
SDL_bool FileIOWriteBinary(FileIO *file);
SDL_bool FileIORead(FileIO *file);
SDL_bool FileIOReadBinary(FileIO *file);
void FileIODestroy(FileIO file);

#endif