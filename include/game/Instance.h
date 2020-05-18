#ifndef INSTANCE_H
#define INSTANCE_H

void InstanceInitialize();
void InstanceUninitialize();

void InstanceReset();

int InstanceGetRound();
int InstanceGetPoints();

void InstanceSetRound(int round);
void InstanceSetPoints(int points);

#endif