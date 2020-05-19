#include "Round.h"

struct
{
    float countdown;
} s_round;

void RoundInitialize()
{
    s_round.countdown = 0.0f;
}

void RoundUninitialize()
{
}

float RoundGetCountdown()
{
    return s_round.countdown;
}

void RoundSetCountdown(float countdown)
{
    s_round.countdown = countdown;
}