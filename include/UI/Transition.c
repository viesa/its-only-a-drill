#include "Transition.h"

#include "Library.h"

typedef struct Transistion
{
    TransitionType type;
    float duration;
    float elapsed;
    int state;
    SDL_bool isDone;

    float saveSlots[10];
    int skipped;
} Transition;

static Transition *trans;

void TransitionInitialize()
{
    trans = MALLOC(Transition);
    ALLOC_ERROR_CHECK(trans);

    trans->duration = 0.0f;
    trans->elapsed = 0.0f;
    trans->state = 0;
    for (int i = 0; i < 10; i++)
        trans->saveSlots[i] = 0.0f;
    trans->skipped = 0;
}

void TransitionUninitialize()
{
    FREE(trans);
}

void TransitionDraw()
{
    if (trans->isDone)
        return;

    if (trans->skipped < 2)
    {
        trans->skipped++;
        return;
    }
    if (trans->elapsed > trans->duration)
        return;

    float dt = ClockGetDeltaTime();

    switch (trans->type)
    {
    case TT_Fade:
    {
        float step = 2.0f * 255.0f / (trans->duration / dt);
        if (trans->elapsed < trans->duration / 2.0f)
            trans->state = 0;
        else
            trans->state = 1;

        switch (trans->state)
        {
        case 0:
            trans->saveSlots[0] += step;
            break;
        case 1:
            trans->saveSlots[0] -= step;
            break;

        default:
            break;
        }
        trans->saveSlots[0] = Clamp(trans->saveSlots[0], 0, 255);

        GraphicsDrawGradientX((SDL_Rect){0, 0, WindowGetWidth(), WindowGetHeight()}, (SDL_Color){0, 0, 0, trans->saveSlots[0]}, (SDL_Color){0, 0, 0, trans->saveSlots[0]});
    }
    case TT_FadeOut:
    {
        float start = 255.0f;

        float to = 0.0f;

        float total = to - start;

        float sinMultiplier = (sinf(trans->elapsed / trans->duration * PI + 3.0f * PI / 2.0f) + 1.0f) / 2.0f;
        sinMultiplier *= sinMultiplier;

        trans->saveSlots[0] = start + total * sinMultiplier;
        trans->saveSlots[0] = Clamp(trans->saveSlots[0], 0, 255);

        GraphicsDrawGradientX((SDL_Rect){0, 0, WindowGetWidth(), WindowGetHeight()}, (SDL_Color){0, 0, 0, trans->saveSlots[0]}, (SDL_Color){0, 0, 0, trans->saveSlots[0]});
    }
    break;
    case TT_MenuToMap:
    {
        float scaleBase = 0.33f;
        float xBase = (float)WindowGetWidth() / 2.0f;
        float yBase = (float)WindowGetHeight() / 2.0f;
        float wBase = (float)WindowGetWidth() / 3.0f;
        float hBase = (float)WindowGetHeight() / 3.0f;
        float alphaBase = 200.0f;

        float scaleTotal = 1.0f - scaleBase;
        float xTotal = -xBase;
        float yTotal = -yBase;
        float wTotal = (float)WindowGetWidth() - wBase;
        float hTotal = (float)WindowGetHeight() - hBase;
        float alphaTotal = 255.0f - alphaBase;

        float sinMultiplier = (sinf(trans->elapsed / trans->duration * PI + 3.0f * PI / 2.0f) + 1.0f) / 2.0f;

        trans->saveSlots[0] = scaleBase + scaleTotal * sinMultiplier;
        if (trans->saveSlots[0] >= 1.0f)
            trans->saveSlots[0] = 1.0f;

        trans->saveSlots[1] = xBase + xTotal * sinMultiplier;
        if (trans->saveSlots[1] <= 0.0f)
            trans->saveSlots[1] = 0.0f;

        trans->saveSlots[2] = yBase + yTotal * sinMultiplier;
        if (trans->saveSlots[2] <= 0.0f)
            trans->saveSlots[2] = 0.0f;

        trans->saveSlots[3] = wBase + wTotal * sinMultiplier;
        if (trans->saveSlots[3] >= (float)WindowGetWidth())
            trans->saveSlots[3] = (float)WindowGetWidth();

        trans->saveSlots[4] = hBase + hTotal * sinMultiplier;
        if (trans->saveSlots[4] >= (float)WindowGetHeight())
            trans->saveSlots[4] = (float)WindowGetHeight();

        trans->saveSlots[5] = alphaBase + alphaTotal * sinMultiplier;
        if (trans->saveSlots[5] >= 255.0f)
            trans->saveSlots[5] = 255.0f;
    }
    break;
    case TT_MapToMenu:
    {
        float scaleBase = 1.0f;
        float xBase = 0.0f;
        float yBase = 0.0f;
        float wBase = (float)WindowGetWidth();
        float hBase = (float)WindowGetHeight();
        float alphaBase = 255.0f;

        float scaleTotal = 0.33f - scaleBase;
        float xTotal = (float)WindowGetWidth() / 2.0f;
        float yTotal = (float)WindowGetHeight() / 2.0f;
        float wTotal = (float)WindowGetWidth() / 3.0f - wBase;
        float hTotal = (float)WindowGetHeight() / 3.0f - hBase;
        float alphaTotal = 200.0f - alphaBase;

        float sinMultiplier = (sinf(trans->elapsed / trans->duration * PI + 3.0f * PI / 2.0f) + 1.0f) / 2.0f;

        trans->saveSlots[0] = scaleBase + scaleTotal * sinMultiplier;
        if (trans->saveSlots[0] <= 0.33f)
            trans->saveSlots[0] = 0.33f;

        trans->saveSlots[1] = xBase + xTotal * sinMultiplier;
        if (trans->saveSlots[1] >= (float)WindowGetWidth() / 2.0f)
            trans->saveSlots[1] = (float)WindowGetWidth() / 2.0f;

        trans->saveSlots[2] = yBase + yTotal * sinMultiplier;
        if (trans->saveSlots[2] >= (float)WindowGetHeight() / 2.0f)
            trans->saveSlots[2] = (float)WindowGetHeight() / 2.0f;

        trans->saveSlots[3] = wBase + wTotal * sinMultiplier;
        if (trans->saveSlots[3] <= (float)WindowGetWidth() / 3.0f)
            trans->saveSlots[3] = (float)WindowGetWidth() / 3.0f;

        trans->saveSlots[4] = hBase + hTotal * sinMultiplier;
        if (trans->saveSlots[4] <= (float)WindowGetHeight() / 3.0f)
            trans->saveSlots[4] = (float)WindowGetHeight() / 3.0f;

        trans->saveSlots[5] = alphaBase + alphaTotal * sinMultiplier;
        if (trans->saveSlots[5] <= 200.0f)
            trans->saveSlots[5] = 200.0f;
    }
    break;

    default:
        return;
    }
    trans->elapsed += dt;

    if (trans->elapsed >= trans->duration)
        trans->isDone = SDL_TRUE;
}

void TransitionStart(TransitionType type, float duration)
{
    trans->type = type;
    trans->duration = duration;
    trans->elapsed = 0.0f;
    trans->state = 0;
    trans->isDone = SDL_FALSE;
    switch (type)
    {
    case TT_Fade:
        trans->saveSlots[0] = 0.0f;
        break;
    case TT_FadeOut:
        trans->saveSlots[0] = 255.0f;
        break;
    case TT_MenuToMap:
        trans->saveSlots[0] = 0.33f;
        trans->saveSlots[1] = (float)WindowGetWidth() / 2.0f;
        trans->saveSlots[2] = (float)WindowGetHeight() / 2.0f;
        trans->saveSlots[3] = (float)WindowGetWidth() / 3.0f;
        trans->saveSlots[4] = (float)WindowGetHeight() / 3.0f;
        trans->saveSlots[5] = 200.0f;
        break;
    case TT_MapToMenu:
        trans->saveSlots[0] = 1.0f;
        trans->saveSlots[1] = 0.0f;
        trans->saveSlots[2] = 0.0f;
        trans->saveSlots[3] = (float)WindowGetWidth();
        trans->saveSlots[4] = (float)WindowGetHeight();
        trans->saveSlots[5] = 255.0f;
        break;
    default:
        break;
    }
}

void TransitionStop()
{
}

void TransitionPause()
{
}

SDL_bool TransitionIsDone()
{
    return trans->isDone;
}

float TransitionGetSaveSlot(int index)
{
    return trans->saveSlots[index];
}