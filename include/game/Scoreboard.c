#include "Scoreboard.h"

struct Scoreboard
{
    // Vector of Scoreboard entries
    Vector *allScores;
} scoreboard;

void ScoreboardInitialize()
{
    scoreboard.allScores = VectorCreate(sizeof(ScoreboardEntry), 5);
}
void ScoreboardUninitialize()
{
    VectorDestroy(scoreboard.allScores);
}

void ScoreboardAddPlayer(int id, char *name)
{
    ScoreboardEntry entry;
    entry.id = id;
    SDL_memset(entry.name, 0, MAX_PLAYERNAME_SIZE);
    int toCopy = strlen(name) < MAX_PLAYERNAME_SIZE ? strlen(name) : MAX_PLAYERNAME_SIZE - 1;
    SDL_memcpy(entry.name, name, toCopy);
    VectorPushBack(scoreboard.allScores, &entry);
}

void ScoreboardAddScore(int id, int score)
{
    for (int i = 0; i < scoreboard.allScores->size; i++)
    {
        ScoreboardEntry *current = &SCOREBOARD_ALLSCORES[i];
        if (current->id == id)
        {
            current->score += score;
            return;
        }
    }
}

void ScoreboardSetScore(int id, int score)
{
    for (int i = 0; i < scoreboard.allScores->size; i++)
    {
        ScoreboardEntry *current = &SCOREBOARD_ALLSCORES[i];
        if (current->id == id)
        {
            current->score = score;
            return;
        }
    }
}

int ScoreboardGetScore(int id)
{
    for (int i = 0; i < scoreboard.allScores->size; i++)
    {
        ScoreboardEntry *current = &SCOREBOARD_ALLSCORES[i];
        if (current->id == id)
        {
            return current->score;
        }
    }
    return -1;
}

ScoreboardEntry *ScoreboardGetAllScores()
{
    return (ScoreboardEntry *)scoreboard.allScores->data;
}
