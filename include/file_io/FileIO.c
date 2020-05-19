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

void FileIODestroy(FileIO *lfile)
{
    FREE(lfile->contents);
    FREE(lfile->path);
}

void FileIOCopy(FileIO *file, void *src, size_t size)
{
    file->contents = (void *)MALLOC_N(char, size);
    ALLOC_ERROR_CHECK(file->contents);
    SDL_memcpy(file->contents, src, size);
    file->size = size;
}

SDL_bool FileIOWriteBinary(FileIO *file)
{
    FILE *f;
    f = fopen(file->path, "wb");
    if (!f)
    {
#ifdef FILEIO_DEBUG
        log_error("fopen error: %s", file->path);
#endif
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
#ifdef FILEIO_DEBUG
        log_error("fopen error: %s", file->path);
#endif
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
        file->contents = MALLOC_N(char, length);
        if (file->contents)
        {
#ifdef FILEIO_DEBUG
            log_error("Malloc error: %s", file->path);
#endif
            fread(file->contents, 1, length, f);
        }
        file->size = length;
        fclose(f);
        return SDL_TRUE;
    }
    file->size = length;
#ifdef FILEIO_DEBUG
    log_warn("FileIO could not read the file %s", file->path);
#endif
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
        file->contents = MALLOC_N(char, length);
        if (file->contents)
        {
#ifdef FILEIO_DEBUG
            log_error("Malloc error: %s", file->path);
#endif
            fread(file->contents, 1, length, f);
        }
        file->size = length;
        fclose(f);
        return SDL_TRUE;
    }
#ifdef FILEIO_DEBUG
    log_error("FileIO error reading file: %s", file->path);
#endif
    return SDL_FALSE;
}