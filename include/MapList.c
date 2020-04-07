#include "MapList.h"

MapList MapListCreate(char *directory)
{
    MapList mapList;
    mapList.allMaps = ListCreate();
    mapList.directory = (char *)SDL_malloc(sizeof(char) * strlen(directory));
    strcpy(mapList.directory, directory);
    MapListUpdate(&mapList);
    return mapList;
}
void MapListDestroy(MapList *mapList)
{
    for (Node *node = mapList->allMaps.front; node; node = node->next)
    {
        MapListEntry *entry = (MapListEntry *)node->data;
        SDL_free(entry->name);
        SDL_free(entry->filename);
    }
    ListDestroy(&mapList->allMaps);
    SDL_free(mapList->directory);
}

void MapListUpdate(MapList *mapList)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(mapList->directory);
    if (d)
    {
        MapListClear(mapList);

        for (int i = 0; (dir = readdir(d)) != NULL; i++)
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
            {
                //Load in mapfile
                size_t bytes = (strlen(mapList->directory) + 1 + strlen(dir->d_name));
                char *fullpath = (char *)SDL_calloc(bytes, sizeof(char) * bytes);
                strcat(fullpath, mapList->directory);
                strcat(fullpath, "/");
                strcat(fullpath, dir->d_name);
                JSON *_new = JSONCreate(fullpath);
                json_object_entry *mapInfo = JSONGetValue(_new, (uint32_t[]){0}, 1)->u.object.values;
                //---------------

                // Push-back this map
                MapListEntry _new_entry;
                _new_entry.uid = mapInfo[0].value->u.integer;
                _new_entry.name = (char *)SDL_malloc(sizeof(char) * mapInfo[1].value->u.string.length);
                strcpy(_new_entry.name, mapInfo[1].value->u.string.ptr);
                _new_entry.filename = (char *)SDL_malloc(sizeof(char) * strlen(fullpath));
                strcpy(_new_entry.filename, fullpath);
                _new_entry.difficulty = mapInfo[2].value->u.integer;
                _new_entry.maxPlayers = mapInfo[3].value->u.integer;
                ListPushBack(&mapList->allMaps, (void *)&_new_entry, sizeof(_new_entry));
                // -------------------

                // Clean up
                SDL_free(fullpath);
                JSONDestroy(_new);
                // --------
            }
    }
    closedir(d);
}

void MapListClear(MapList *mapList)
{
    for (Node *node = mapList->allMaps.front; node; node = node->next)
    {
        MapListEntry *entry = (MapListEntry *)node->data;
        SDL_free(entry->name);
    }
    ListClear(&mapList->allMaps);
}