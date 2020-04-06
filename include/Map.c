#include "Map.h"

#include "core/Library.h"

Map *MapCreate(JSON *mapdata)
{
    Map *map = (Map *)SDL_malloc(sizeof(Map));
    map->contents = NULL;

    //Converts mapdata into entities
    uint32_t layers = JSONGetValue(mapdata, (uint32_t[]){0}, 1)->u.integer;
    map->n = layers;

    map->contents = (Entity *)SDL_malloc(sizeof(Entity) * layers);
    for (uint32_t i = 0; i < layers; i++)
    {
        json_value *current = JSONGetValue(mapdata, (uint32_t[]){1, i}, 2);
        json_object_entry *entries = current->u.object.values;

        char *type_str = entries[0].value->u.string.ptr;
        EntityPresets type;
        if (!strcmp(type_str, "player"))
            type = EntityPlayerSpawn;
        else
            type = EntityMapObject;
        Vec2 position = Vec2Create((float)entries[2].value->u.integer, (float)entries[3].value->u.integer);
        int width = entries[4].value->u.integer;
        int height = entries[5].value->u.integer;
        float mass = (float)entries[6].value->u.integer;
        SDL_bool collider = (SDL_bool)entries[7].value->u.integer;
        int rotation = entries[8].value->u.integer;

        int src_x = entries[9].value->u.object.values[0].value->u.integer;
        int src_y = entries[9].value->u.object.values[1].value->u.integer;
        int src_w = entries[9].value->u.object.values[2].value->u.integer;
        int src_h = entries[9].value->u.object.values[3].value->u.integer;
        SDL_Rect src = {src_x, src_y, src_w, src_h};

        Entity _new_ = EntityCreate(position, 0, 0, type, 0);
        _new_.drawable.dst.w = width;
        _new_.drawable.dst.h = height;
        _new_.drawable.rot = rotation;
        _new_.drawable.rot_anchor = RectMid(_new_.drawable.dst);
        _new_.mass = mass;
        _new_.isCollider = collider;
        _new_.drawable.src = src;

        SDL_memcpy(&map->contents[i], &_new_, sizeof(Entity));
    }
    return map;
}
void MapDestroy(Map *map)
{
    SDL_free(map->contents);
    SDL_free(map);
}