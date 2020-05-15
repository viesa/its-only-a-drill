#ifndef FILELOADER_H
#define FILELOADER_H

#include "Dependencies.h"

typedef struct FileIO
{
    char *path;
    void *contents;
    int size;
} FileIO;
// Returns a FileIO struct with path allocated based on filename
FileIO FileIOCreate(char *filename);
// Writes to path with contents (size isnt a consideration)
SDL_bool FileIOWrite(FileIO *file);
// Writes to path with contents (binary format) (size isnt a consideration)
SDL_bool FileIOWriteBinary(FileIO *file);
// Reads a string from path (size is the returned length)
// Contents is allocated
SDL_bool FileIORead(FileIO *file);
// Reads a binary file from path (size is the file size)
// Contents is allocated
SDL_bool FileIOReadBinary(FileIO *file);
// Free the path and contents
void FileIODestroy(FileIO *file);

#endif