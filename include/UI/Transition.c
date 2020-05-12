#include "Transition.h"

#include "Library.h"

struct
{
    Graphics *gfx;
    Font *font;
    TransitionType type;
    float duration;
    float elapsed;
    int state;
    SDL_bool isDone;

    float saveSlot1;
    float saveSlot2;
    float saveSlot3;
    float saveSlot4;
    float saveSlot5;
    float saveSlot6;
    int skipped;
} trans;

void TransitionInitalize(Graphics *gfx, Font *font)
{
    trans.gfx = gfx;
    trans.font = font;
    trans.duration = 0.0f;
    trans.elapsed = 0.0f;
    trans.state = 0;
    trans.saveSlot1 = 0.0f;
    trans.saveSlot2 = 0.0f;
    trans.saveSlot3 = 0.0f;
    trans.saveSlot4 = 0.0f;
    trans.saveSlot5 = 0.0f;
    trans.saveSlot6 = 0.0f;
    trans.skipped = 0;
}

void TransitionDraw()
{
    if (trans.isDone)
        return;

    if (trans.skipped < 2)
    {
        trans.skipped++;
        return;
    }
    if (trans.elapsed > trans.duration)
        return;

    float dt = ClockGetDeltaTime();

    switch (trans.type)
    {
    case TT_Fade:
    {
        float step = 2.0f * 255.0f / (trans.duration / dt);
        if (trans.elapsed < trans.duration / 2.0f)
            trans.state = 0;
        else
            trans.state = 1;

        switch (trans.state)
        {
        case 0:
            trans.saveSlot1 += step;
            break;
        case 1:
            trans.saveSlot1 -= step;
            break;

        default:
            break;
        }
        trans.saveSlot1 = Clamp(trans.saveSlot1, 0, 255);

        GraphicsDrawGradientX(trans.gfx, (SDL_Rect){0, 0, trans.gfx->window->width, trans.gfx->window->height}, (SDL_Color){0, 0, 0, trans.saveSlot1}, (SDL_Color){0, 0, 0, trans.saveSlot1});
    }
    case TT_FadeOut:
    {
        float step = 255.0f / (trans.duration / dt);
        if (trans.elapsed < 1)
            trans.state = 0;
        else
            trans.state = 1;

        switch (trans.state)
        {
        case 0:
            break;
        case 1:
            trans.saveSlot1 -= step;
            break;

        default:
            break;
        }
        trans.saveSlot1 = Clamp(trans.saveSlot1, 0, 255);

        GraphicsDrawGradientX(trans.gfx, (SDL_Rect){0, 0, trans.gfx->window->width, trans.gfx->window->height}, (SDL_Color){0, 0, 0, trans.saveSlot1}, (SDL_Color){0, 0, 0, trans.saveSlot1});
    }
    break;
    case TT_StartMap:
    {
        float scaleTotal = 1.0f - 0.33f;
        float xTotal = (float)trans.gfx->window->width / 2.0f;
        float yTotal = (float)trans.gfx->window->height / 2.0f;
        float wTotal = (float)trans.gfx->window->width - (float)trans.gfx->window->width / 3.0f;
        float hTotal = (float)trans.gfx->window->height - (float)trans.gfx->window->height / 3.0f;
        float alphaTotal = 255.0f - 200;

        float scaleDelta = scaleTotal / trans.duration;
        float xDelta = xTotal / trans.duration;
        float yDelta = yTotal / trans.duration;
        float wDelta = wTotal / trans.duration;
        float hDelta = hTotal / trans.duration;
        float alphaDelta = alphaTotal / trans.duration;

        trans.saveSlot1 += scaleDelta * dt;
        if (trans.saveSlot1 >= 1.0f)
            trans.saveSlot1 = 1.0f;

        trans.saveSlot2 -= xDelta * dt;
        if (trans.saveSlot2 <= 0.0f)
            trans.saveSlot2 = 0.0f;

        trans.saveSlot3 -= yDelta * dt;
        if (trans.saveSlot3 <= 0.0f)
            trans.saveSlot3 = 0.0f;

        trans.saveSlot4 += wDelta * dt;
        if (trans.saveSlot4 >= (float)trans.gfx->window->width)
            trans.saveSlot4 = (float)trans.gfx->window->width;

        trans.saveSlot5 += hDelta * dt;
        if (trans.saveSlot5 >= (float)trans.gfx->window->height)
            trans.saveSlot5 = (float)trans.gfx->window->height;

        trans.saveSlot6 += alphaDelta * dt;
        if (trans.saveSlot6 >= 255.0f)
            trans.saveSlot6 = 255.0f;
    }
    break;
    default:
        return;
    }
    trans.elapsed += dt;

    if (trans.elapsed >= trans.duration)
        trans.isDone = SDL_TRUE;
}

void TransitionStart(TransitionType type, float duration)
{
    trans.type = type;
    trans.duration = duration;
    trans.elapsed = 0.0f;
    trans.state = 0;
    trans.isDone = SDL_FALSE;
    switch (type)
    {
    case TT_Fade:
        trans.saveSlot1 = 0.0f;
        break;
    case TT_FadeOut:
        trans.saveSlot1 = 255.0f;
        break;
    case TT_StartMap:
        trans.saveSlot1 = 0.33f;
        trans.saveSlot2 = (float)trans.gfx->window->width / 2.0f;
        trans.saveSlot3 = (float)trans.gfx->window->height / 2.0f;
        trans.saveSlot4 = (float)trans.gfx->window->width / 3.0f;
        trans.saveSlot5 = (float)trans.gfx->window->height / 3.0f;
        trans.saveSlot6 = 200.0f;
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
    return trans.isDone;
}

float TransitionGetSaveSlot(int index)
{
    float *firstSaveSlot = &trans.saveSlot1;
    return *(firstSaveSlot + index);
}