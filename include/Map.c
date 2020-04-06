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
        else if (!strcmp(type_str, "gun"))
            type = EntityGun;
        Vec2 position = Vec2Create((float)entries[2].value->u.integer, (float)entries[3].value->u.integer);
        int width = entries[4].value->u.integer;
        int height = entries[5].value->u.integer;
        float mass = (float)entries[6].value->u.integer;
        SDL_bool collider = (SDL_bool)entries[7].value->u.integer;
        int rotation = entries[8].value->u.integer;

        Entity e = EntityCreate(position, type, 0);
        e.drawable.dst.w = width;
        e.drawable.dst.h = height;
        e.drawable.rot = rotation;
        e.drawable.rot_anchor = RectMid(e.drawable.dst);
        e.mass = mass;
        e.isCollider = collider;

        SDL_memcpy(&map->contents[i], &e, sizeof(Entity));
    }
    return map;
}
void MapDestroy(Map *map)
{
}