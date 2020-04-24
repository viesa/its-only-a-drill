#include "Map.h"

#include "core/Library.h"

Map MapCreate(JSON *mapdata)
{
    Map error_map = {NULL, 0};
    Map map;
    map.n = 0;
    if (mapdata == NULL ||
        mapdata->value->type != json_object ||
        mapdata->value->u.object.length != MAIN_LENGTH)
    {
        log_error("Could not create map from mapfile: JSON-data was badly formatted (Main)");
        MapDestroy(&map);
        return error_map;
    }

    json_value *mapInfo = JSONGetValue(mapdata, (uint32_t[]){MAPINFO_INDEX}, 1);
    if (mapInfo == NULL ||
        mapInfo->type != json_object ||
        mapInfo->u.object.length != MAPINFO_LENGTH)
    {
        log_error("Could not create map from mapfile: JSON-data was badly formatted (MapInfo)");
        MapDestroy(&map);
        return error_map;
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
        log_error("Could not create map from mapfile: JSON-data was badly formatted (MapInfo)");
        return error_map;
    }

    json_value *layers = JSONGetValue(mapdata, (uint32_t[]){LAYER_INDEX}, 1);
    if (layers == NULL ||
        layers->type != json_integer ||
        layers->u.integer != JSONGetValue(mapdata, (uint32_t[]){LIST_INDEX}, 1)->u.array.length)
    {
        log_error("Could not create map from mapfile: JSON-data was badly formatted (Layers)");
        MapDestroy(&map);
        return error_map;
    }
    map.n = layers->u.integer;

    map.contents = MALLOC_N(EntityIndexP, map.n);
    for (uint32_t i = 0; i < map.n; i++)
    {
        json_value *current = JSONGetValue(mapdata, (uint32_t[]){LIST_INDEX, i}, 2);
        if (current == NULL ||
            current->type != json_object ||
            current->u.object.length != LIST_LENGTH)
        {
            log_error("Could not create map from mapfile: JSON-data was badly formatted (List)");
            MapDestroy(&map);
            return error_map;
        }

        json_object_entry *entries = current->u.object.values;

        // Sanity check for all the value types in the JSON-file
        for (uint32_t i = 0; i < LIST_LENGTH; i++)
            if (i == MAPINFO_INDEX)
            {
                if (entries[i].value->type != json_string)
                {
                    badLoad = SDL_TRUE;
                    break;
                }
            }
            else if (i == SRC_INDEX)
            {
                for (uint32_t j = 0; j < 4; j++)
                {
                    if (entries[i].value->u.object.values[j].value->type != json_integer)
                    {
                        badLoad = SDL_TRUE;
                        break;
                    }
                }
            }
            else
            {
                if (entries[i].value->type != json_integer)
                {
                    badLoad = SDL_TRUE;
                    break;
                }
            }
        if (badLoad)
        {
            log_error("Could not create map from mapfile: JSON-data was badly formatted (LIST CONTENTS)");
            MapDestroy(&map);
            return error_map;
        }
        //----------------------------------------------------

        if (entries[SRC_INDEX].value->type != json_object ||
            entries[SRC_INDEX].value->u.object.length != SRC_LENGTH)
        {
            log_error("Could not create map from mapfile: JSON-data was badly formatted (SRC)");
            MapDestroy(&map);
            return error_map;
        }

        char *type_str = entries[0].value->u.string.ptr;
        EntityType type;
        if (!strcmp(type_str, "player"))
            type = ET_PlayerSpawn;
        else
            type = ET_MapObject;

        Vec2 position = Vec2Create((float)entries[2].value->u.integer, (float)entries[3].value->u.integer);
        int width = entries[4].value->u.integer;
        int height = entries[5].value->u.integer;
        float mass = (float)entries[6].value->u.integer;
        SDL_bool collider = (SDL_bool)entries[7].value->u.integer;
        int rotation = entries[8].value->u.integer;

        int src_x = entries[SRC_INDEX].value->u.object.values[0].value->u.integer;
        int src_y = entries[SRC_INDEX].value->u.object.values[1].value->u.integer;
        int src_w = entries[SRC_INDEX].value->u.object.values[2].value->u.integer;
        int src_h = entries[SRC_INDEX].value->u.object.values[3].value->u.integer;

        SDL_Rect dst = {(int)position.x, (int)position.y, width, height};
        SDL_Rect src = {src_x, src_y, src_w, src_h};

        EntityIndexP index = EntityManagerAdd(type, position);
        ENTITY_ARRAY[*index].drawables[0].dst = dst;
        ENTITY_ARRAY[*index].drawables[0].src = src;
        ENTITY_ARRAY[*index].drawables[0].rot = rotation;
        ENTITY_ARRAY[*index].drawables[0].rot_anchor = RectMid(ENTITY_ARRAY[*index].drawables[0].dst);
        ENTITY_ARRAY[*index].mass = mass;
        ENTITY_ARRAY[*index].isCollider = collider;
        ENTITY_ARRAY[*index].hitboxIndex = 0;
        ENTITY_ARRAY[*index].nDrawables = 1;

        map.contents[i] = index;
    }
    return map;
}
void MapDestroy(Map *map)
{
    EntityManagerRemoveRange(map->contents[0], map->contents[map->n - 1]);

    if (map->contents)
        SDL_free(map->contents);
    map->contents = NULL;
    map->n = 0;
}

void MapDraw(Map *map, Camera *camera)
{
    if (map->contents)
        for (int i = 0; i < map->n; i++)
        {
            EntityDrawIndex(map->contents[i], camera);
        }
}