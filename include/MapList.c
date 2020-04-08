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
                //Load in mapinfo from mapfile
                size_t bytes = (strlen(mapList->directory) + 1 + strlen(dir->d_name));
                char *fullpath = (char *)SDL_calloc(bytes, sizeof(char) * bytes);
                strcat(fullpath, mapList->directory);
                strcat(fullpath, "/");
                strcat(fullpath, dir->d_name);
                JSON *_new = JSONCreate(fullpath);

                if (_new == NULL ||
                    _new->value->type != json_object ||
                    _new->value->u.object.length != MAIN_LENGTH)
                {
                    log_error("Could not load mapinfo from mapfile: JSON-data was badly formatted (Main)");
                    continue;
                }

                json_value *mapInfo = JSONGetValue(_new, (uint32_t[]){0}, 1);

                if (mapInfo == NULL ||
                    mapInfo->type != json_object ||
                    mapInfo->u.object.length != MAPINFO_LENGTH)
                {
                    log_error("Could not load mapinfo from mapfile: JSON-data was badly formatted (MapInfo)");
                    continue;
                }

                SDL_bool badLoad = SDL_FALSE;
                json_object_entry *mapInfoEntries = mapInfo->u.object.values;
                for (uint32_t i = 0; i < MAPINFO_LENGTH; i++)
                {
                    if (i == MAPINFO_STRING_INDEX)
                    {
                        if (mapInfoEntries[i].value->type != json_string)
                        {
                            badLoad = SDL_TRUE;
                            break;
                        }
                    }
                    else
                    {
                        if (mapInfoEntries[i].value->type != json_integer)
                        {
                            badLoad = SDL_TRUE;
                            break;
                        }
                    }
                }
                if (badLoad)
                {
                    log_error("Could not load mapinfo from mapfile: JSON-data was badly formatted (MapInfo)");
                    continue;
                }
                //---------------

                // Push-back this map
                MapListEntry _new_entry;
                _new_entry.uid = mapInfoEntries[0].value->u.integer;
                _new_entry.name = (char *)SDL_malloc(sizeof(char) * mapInfoEntries[1].value->u.string.length);
                strcpy(_new_entry.name, mapInfoEntries[1].value->u.string.ptr);
                _new_entry.filename = (char *)SDL_malloc(sizeof(char) * strlen(fullpath));
                strcpy(_new_entry.filename, fullpath);
                _new_entry.difficulty = mapInfoEntries[2].value->u.integer;
                _new_entry.maxPlayers = mapInfoEntries[3].value->u.integer;
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