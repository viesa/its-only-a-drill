#include "Transition.h"

struct
{
    Graphics *gfx;
    Font *font;
    TransitionType type;
    float duration;
    float elapsed;
    int state;
    float saveSlot1;
    float saveSlot2;
    float saveSlot3;
    float saveSlot4;
    float saveSlot5;
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
    trans.skipped = 0;
}

void TransitionDraw(Clock *clock)
{
    if (trans.skipped < 2)
    {
        trans.skipped++;
        return;
    }
    if (trans.elapsed > trans.duration)
        return;

    float dt = ClockGetDeltaTime(clock);

    switch (trans.type)
    {
    case TT_Swoosh:
    {

        //Left alpha channel: trans.saveSlot1;
        //Right alpha channel: trans.saveSlot2;
        float step = 4 * 255.0f / (trans.duration / dt);
        if (trans.elapsed < trans.duration / 4)
            trans.state = 0;
        else if (trans.elapsed < trans.duration / 2)
            trans.state = 1;
        else if (trans.elapsed < 3 * trans.duration / 4)
            trans.state = 2;
        else
            trans.state = 3;

        float betweenzao = (trans.elapsed - trans.state * trans.duration / 4);
        float sinFactor = (sin(2 * PI * betweenzao) + 1);
        log_info("sin: %f, b:%f", sinFactor, betweenzao);
        switch (trans.state)
        {
        case 0:
            trans.saveSlot1 += step * sinFactor;
            break;
        case 1:
            trans.saveSlot2 += step * sinFactor;
            break;
        case 2:
            trans.saveSlot1 -= step * sinFactor;
            break;
        case 3:
            trans.saveSlot2 -= step * sinFactor;
            break;

        default:
            break;
        }
        trans.saveSlot1 = Clamp(trans.saveSlot1, 0, 255);
        trans.saveSlot2 = Clamp(trans.saveSlot2, 0, 255);

        GraphicsDrawGradientX(trans.gfx, (SDL_Rect){0, 0, trans.gfx->window->width, trans.gfx->window->height}, (SDL_Color){0, 0, 0, trans.saveSlot1}, (SDL_Color){0, 0, 0, trans.saveSlot2});
    }
    break;
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

    default:
        return;
    }

    trans.elapsed += dt;
}

void TransitionStart(TransitionType type, float duration)
{
    ;
    trans.type = type;
    trans.duration = duration;
    trans.elapsed = 0.0f;
    trans.state = 0;
    switch (type)
    {
    case TT_Swoosh:
        trans.saveSlot1 = 0.0f;
        trans.saveSlot2 = 0.0f;
        break;
    case TT_Fade:
        trans.saveSlot1 = 0.0f;
        break;
    case TT_FadeOut:
        trans.saveSlot1 = 255.0f;
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