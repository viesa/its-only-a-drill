#include "Instance.h"

struct Instance
{
    int currentRound;
    int points;
} instance;

void InstanceInitialize()
{
    instance.currentRound = 0;
    instance.points = 0;
}
void InstanceUninitialize()
{
}

void InstanceReset()
{
    instance.currentRound = 0;
    instance.points = 0;
}

int InstanceGetRound()
{
    return instance.currentRound;
}
int InstanceGetPoints()
{
    return instance.points;
}

void InstanceSetRound(int round)
{
    instance.currentRound = round;
}
void InstanceSetPoints(int points)
{
    instance.points = points;
}