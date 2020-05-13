#include "FileIO.h"

#include "sys/stat.h"

FileIO FileIOCreate(char filename[])
{
    FileIO lfile;
    lfile.contents = NULL;
    lfile.path = (char *)MALLOC_N(char, strlen(filename) + 1);
    SDL_memcpy(lfile.path, filename, strlen(filename) + 1);
    return lfile;
}
SDL_bool FileIOWriteBinary(FileIO *file)
{
    FILE *f;
    f = fopen(file->path, "wb");
    if (!f)
    {
        log_error("fopen error");
        return SDL_FALSE;
    }
    fwrite(file->contents, file->size, 1, f);
    fclose(f);
    return SDL_TRUE;
}
SDL_bool FileIOWrite(FileIO *file)
{
    FILE *f;
    f = fopen(file->path, "w");
    if (!f)
    {
        log_error("fopen error");
        return SDL_FALSE;
    }
    fprintf(f, "%s", (char *)file->contents);
    fclose(f);
    return SDL_TRUE;
}
SDL_bool FileIOReadBinary(FileIO *file)
{
    long length = 0;
    FILE *f = fopen(file->path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        file->contents = SDL_malloc(length);
        if (file->contents)
        {
            fread(file->contents, 1, length, f);
        }
        file->size = length;
        fclose(f);
        return SDL_TRUE;
    }
    file->size = length;
    log_warn("FileIO couldnt read the file");
    return SDL_FALSE;
}
SDL_bool FileIORead(FileIO *file)
{
    long length = 0;
    FILE *f = fopen(file->path, "r");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        file->contents = (char *)malloc(length);
        if (file->contents)
        {
            fread(file->contents, 1, length, f);
        }
        file->size = length;
        fclose(f);
        return SDL_TRUE;
    }
    log_error("FileIO error reading file");
    return SDL_FALSE;
}
void FileIODestroy(FileIO lfile)
{
    FREE(lfile.contents);
    FREE(lfile.path);
}
