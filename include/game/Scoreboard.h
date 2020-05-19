#ifndef SCORE_H
#define SCORE_H

#include "Vector.h"

#define SCOREBOARD_ALLSCORES ScoreboardGetAllScores()

typedef struct ScoreboardEntry
{
    int id;
    char name[MAX_PLAYERNAME_SIZE];
    int score;
} ScoreboardEntry;

void ScoreboardInitialize();
void ScoreboardUninitialize();

void ScoreboardAddPlayer(int id, char *name);

void ScoreboardAddScore(int id, int score);
void ScoreboardSetScore(int id, int score);
int ScoreboardGetScore(int id);

ScoreboardEntry *ScoreboardGetAllScores();

#endif