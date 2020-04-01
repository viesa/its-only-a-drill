#ifndef SCORE_H
#define SCORE_H
#include "Entity.h"

void ScoreCreate(int id); 
int ScoreInfo(int id);
void ScoreIncrement(int plus,int id);
void ScoreMultiChange(double new,int id);
#endif