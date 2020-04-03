#ifndef SCORE_H
#define SCORE_H

//Skapar en scorebord för en spelare
void ScoreCreate(int id);

//Får score om en spelare
int ScoreInfo(int id);

//Ökar score med plus * multi
void ScoreIncrement(int plus, int id);

//Ändrar multiplier för en spelare
void ScoreMultiChange(double new, int id);
#endif