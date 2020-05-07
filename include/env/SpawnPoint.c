#include "SpawnPoint.h"

SpawnPoint SpawnPointCreate(Vec2 position, int id)
{
    return (SpawnPoint){position, id};
}