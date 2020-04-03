#include "Score.h"
#include <stdio.h>

struct Score
{
    int score;
    double multi;
} Player[4];

void ScoreCreate(int id)
{
    Player[id].score = 0;
    Player[id].multi = 1.0;
}

int ScoreInfo(int id)
{
    return Player[id].score;
}

void ScoreIncrement(int plus, int id)
{
    Player[id].score += (plus * Player[id].multi);
}

void ScoreMultiChange(double new, int id)
{
    Player[id].multi = new;
}
