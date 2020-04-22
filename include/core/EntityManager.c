#include "EntityManager.h"
EntityManager EntityManagerCreate()
{
    EntityManager m;
    m.nrEntities = 0;
    return m;
}
void EntityManagerAdd(EntityManager *m, Entity ent)
{
    m->nrEntities++;
    m->entities = (Entity *)SDL_realloc(m->entities, sizeof(Entity) * m->nrEntities);
    m->entities[m->nrEntities - 1] = ent;
}
void EntityManagerRemove(EntityManager *m, int index)
{
    if (m->nrEntities < index)
        return;
    for (int i = index; i < m->nrEntities; i++)
    {
        m->entities[i] = m->entities[i + 1];
    }
    m->nrEntities--;
    m->entities = SDL_realloc(m->entities, sizeof(Entity) * m->nrEntities);
}
void EntityManagerDestroy(EntityManager *m)
{
    SDL_free(m->entities);
}