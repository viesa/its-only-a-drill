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

    float saveSlots[10];
    int skipped;
} trans;

void TransitionInitalize(Graphics *gfx, Font *font)
{
    trans.gfx = gfx;
    trans.font = font;
    trans.duration = 0.0f;
    trans.elapsed = 0.0f;
    trans.state = 0;
    for (int i = 0; i < 10; i++)
        trans.saveSlots[i] = 0.0f;
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
            trans.saveSlots[0] += step;
            break;
        case 1:
            trans.saveSlots[0] -= step;
            break;

        default:
            break;
        }
        trans.saveSlots[0] = Clamp(trans.saveSlots[0], 0, 255);

        GraphicsDrawGradientX(trans.gfx, (SDL_Rect){0, 0, trans.gfx->window->width, trans.gfx->window->height}, (SDL_Color){0, 0, 0, trans.saveSlots[0]}, (SDL_Color){0, 0, 0, trans.saveSlots[0]});
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
            trans.saveSlots[0] -= step;
            break;

        default:
            break;
        }
        trans.saveSlots[0] = Clamp(trans.saveSlots[0], 0, 255);

        GraphicsDrawGradientX(trans.gfx, (SDL_Rect){0, 0, trans.gfx->window->width, trans.gfx->window->height}, (SDL_Color){0, 0, 0, trans.saveSlots[0]}, (SDL_Color){0, 0, 0, trans.saveSlots[0]});
    }
    break;
    case TT_MenuToMap:
    {
        float scaleBase = 0.33f;
        float xBase = (float)trans.gfx->window->width / 2.0f;
        float yBase = (float)trans.gfx->window->height / 2.0f;
        float wBase = (float)trans.gfx->window->width / 3.0f;
        float hBase = (float)trans.gfx->window->height / 3.0f;
        float alphaBase = 200.0f;

        float scaleTotal = 1.0f - scaleBase;
        float xTotal = -xBase;
        float yTotal = -yBase;
        float wTotal = (float)trans.gfx->window->width - wBase;
        float hTotal = (float)trans.gfx->window->height - hBase;
        float alphaTotal = 255.0f - alphaBase;

        float sinMultiplier = (sinf(trans.elapsed / trans.duration * PI + 3.0f * PI / 2.0f) + 1.0f) / 2.0f;

        trans.saveSlots[0] = scaleBase + scaleTotal * sinMultiplier;
        if (trans.saveSlots[0] >= 1.0f)
            trans.saveSlots[0] = 1.0f;

        trans.saveSlots[1] = xBase + xTotal * sinMultiplier;
        if (trans.saveSlots[1] <= 0.0f)
            trans.saveSlots[1] = 0.0f;

        trans.saveSlots[2] = yBase + yTotal * sinMultiplier;
        if (trans.saveSlots[2] <= 0.0f)
            trans.saveSlots[2] = 0.0f;

        trans.saveSlots[3] = wBase + wTotal * sinMultiplier;
        if (trans.saveSlots[3] >= (float)trans.gfx->window->width)
            trans.saveSlots[3] = (float)trans.gfx->window->width;

        trans.saveSlots[4] = hBase + hTotal * sinMultiplier;
        if (trans.saveSlots[4] >= (float)trans.gfx->window->height)
            trans.saveSlots[4] = (float)trans.gfx->window->height;

        trans.saveSlots[5] = alphaBase + alphaTotal * sinMultiplier;
        if (trans.saveSlots[5] >= 255.0f)
            trans.saveSlots[5] = 255.0f;
    }
    break;
    case TT_MapToMenu:
    {
        float scaleBase = 1.0f;
        float xBase = 0.0f;
        float yBase = 0.0f;
        float wBase = (float)trans.gfx->window->width;
        float hBase = (float)trans.gfx->window->height;
        float alphaBase = 255.0f;

        float scaleTotal = 0.33f - scaleBase;
        float xTotal = (float)trans.gfx->window->width / 2.0f;
        float yTotal = (float)trans.gfx->window->height / 2.0f;
        float wTotal = (float)trans.gfx->window->width / 3.0f - wBase;
        float hTotal = (float)trans.gfx->window->height / 3.0f - hBase;
        float alphaTotal = 200.0f - alphaBase;

        float sinMultiplier = (sinf(trans.elapsed / trans.duration * PI + 3.0f * PI / 2.0f) + 1.0f) / 2.0f;

        trans.saveSlots[0] = scaleBase + scaleTotal * sinMultiplier;
        if (trans.saveSlots[0] <= 0.33f)
            trans.saveSlots[0] = 0.33f;

        trans.saveSlots[1] = xBase + xTotal * sinMultiplier;
        if (trans.saveSlots[1] >= (float)trans.gfx->window->width / 2.0f)
            trans.saveSlots[1] = (float)trans.gfx->window->width / 2.0f;

        trans.saveSlots[2] = yBase + yTotal * sinMultiplier;
        if (trans.saveSlots[2] >= (float)trans.gfx->window->height / 2.0f)
            trans.saveSlots[2] = (float)trans.gfx->window->height / 2.0f;

        trans.saveSlots[3] = wBase + wTotal * sinMultiplier;
        if (trans.saveSlots[3] <= (float)trans.gfx->window->width / 3.0f)
            trans.saveSlots[3] = (float)trans.gfx->window->width / 3.0f;

        trans.saveSlots[4] = hBase + hTotal * sinMultiplier;
        if (trans.saveSlots[4] <= (float)trans.gfx->window->height / 3.0f)
            trans.saveSlots[4] = (float)trans.gfx->window->height / 3.0f;

        trans.saveSlots[5] = alphaBase + alphaTotal * sinMultiplier;
        if (trans.saveSlots[5] <= 200.0f)
            trans.saveSlots[5] = 200.0f;
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
        trans.saveSlots[0] = 0.0f;
        break;
    case TT_FadeOut:
        trans.saveSlots[0] = 255.0f;
        break;
    case TT_MenuToMap:
        trans.saveSlots[0] = 0.33f;
        trans.saveSlots[1] = (float)trans.gfx->window->width / 2.0f;
        trans.saveSlots[2] = (float)trans.gfx->window->height / 2.0f;
        trans.saveSlots[3] = (float)trans.gfx->window->width / 3.0f;
        trans.saveSlots[4] = (float)trans.gfx->window->height / 3.0f;
        trans.saveSlots[5] = 200.0f;
        break;
    case TT_MapToMenu:
        trans.saveSlots[0] = 1.0f;
        trans.saveSlots[1] = 0.0f;
        trans.saveSlots[2] = 0.0f;
        trans.saveSlots[3] = (float)trans.gfx->window->width;
        trans.saveSlots[4] = (float)trans.gfx->window->height;
        trans.saveSlots[5] = 255.0f;
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
    return trans.saveSlots[index];
}