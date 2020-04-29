#include "LoadedFile.h"

#include "sys/stat.h"

LoadedFile LoadedFileCreate(char *filename)
{
    LoadedFile lfile;
    LoadedFile badFile = {NULL, 0};
    FILE *fp;
    struct stat filestatus;

    if (stat(filename, &filestatus) != 0)
    {
        log_error("File Loading error: File %s not found", filename);
        return badFile;
    }
    lfile.size = filestatus.st_size;
    lfile.contents = MALLOC_N(char, filestatus.st_size);

    if (lfile.contents == NULL)
    {
        log_error("File Loading error: Unable to allocate %d bytes", lfile.size);
        return badFile;
    }

    fp = fopen(filename, "rt");
    if (fp == NULL)
    {
        log_error("File Loading error: Unable to open %s", filename);
        fclose(fp);
        SDL_free(lfile.contents);
        return badFile;
    }
    if (fread(lfile.contents, lfile.size, 1, fp) != 1)
    {
        log_error("File Loading error: Unable to read contents of %s", filename);
        fclose(fp);
        SDL_free(lfile.contents);
        return badFile;
    }
    fclose(fp);
    return lfile;
}

void LoadedFileDestroy(LoadedFile *lfile)
{
    FREE(lfile->contents);
}
