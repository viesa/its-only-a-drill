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

void ScoreboardClear();

void ScoreboardAddPlayer(int id, char *name, int startingScore);

void ScoreboardAddScore(int id, int score);
void ScoreboardSetScore(int id, int score);
int ScoreboardGetScore(int id);
size_t ScoreboardGetNumPlayers();

ScoreboardEntry *ScoreboardGetAllScores();

#endif