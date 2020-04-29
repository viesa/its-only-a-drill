#ifndef FILELOADER_H
#define FILELOADER_H

#include "Dependencies.h"

typedef struct LoadedFile
{
    char *contents;
    int size;
} LoadedFile;

LoadedFile LoadedFileCreate(char *filename);
void LoadedFileDestroy(LoadedFile *file);

#endif