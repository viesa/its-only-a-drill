#include "Input.h"

#define NUM_MOUSE_EVENTS 6

struct Input
{
    SDL_bool m_keymap[SDL_NUM_SCANCODES];
    SDL_bool m_prevKeymap[SDL_NUM_SCANCODES];

    SDL_bool m_mousemap[NUM_MOUSE_EVENTS];
    SDL_bool m_prevMousemap[NUM_MOUSE_EVENTS];

    Vec2 m_mousePosition;
    int textLen;
    char text[16];
};

Input *InputCreate()
{
    Input *ret = MALLOC(Input);
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        ret->m_keymap[i] = SDL_FALSE;
        ret->m_prevKeymap[i] = SDL_FALSE;
    }
    for (int i = 0; i < NUM_MOUSE_EVENTS; i++)
    {
        ret->m_mousemap[i] = SDL_FALSE;
        ret->m_prevMousemap[i] = SDL_FALSE;
    }
    ret->textLen = 0;
    SDL_memset(ret->text, 0, 16);
    return ret;
}
void InputDestroy(Input *input)
{
    SDL_free(input);
}

void InputUpdate(Input *input)
{
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        input->m_prevKeymap[i] = input->m_keymap[i];
    }
    for (int i = 0; i < NUM_MOUSE_EVENTS; i++)
    {
        input->m_prevMousemap[i] = input->m_mousemap[i];
    }
}

void InputKeyDown(Input *input, const SDL_Scancode key)
{
    input->m_keymap[key] = SDL_TRUE;
}

void InputKeyUp(Input *input, const SDL_Scancode key)
{
    input->m_keymap[key] = SDL_FALSE;
}

void InputMouseDown(Input *input, const MouseCode code)
{
    input->m_mousemap[code] = SDL_TRUE;
}

void InputMouseUp(Input *input, const MouseCode code)
{
    input->m_mousemap[code] = SDL_FALSE;
}

void InputMouseMove(Input *input, Vec2 pos)
{
    input->m_mousePosition = pos;
}

SDL_bool InputIsKeyDown(Input *input, const SDL_Scancode key)
{
    return input->m_keymap[key];
}

SDL_bool InputIsKeyPressed(Input *input, const SDL_Scancode key)
{
    return input->m_keymap[key] && !input->m_prevKeymap[key];
}

SDL_bool InputIsKeyReleased(Input *input, const SDL_Scancode key)
{
    return !input->m_keymap[key] && input->m_prevKeymap[key];
}

SDL_bool InputIsAnyKeyDown(Input *input)
{
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        if (input->m_keymap[i])
            return SDL_TRUE;
    }
    return SDL_FALSE;
}

SDL_bool InputIsMouseDown(Input *input, const MouseCode code)
{
    return input->m_mousemap[code];
}

SDL_bool InputIsMousePressed(Input *input, const MouseCode code)
{
    return input->m_mousemap[code] && !input->m_prevMousemap[code];
}

SDL_bool InputIsMouseReleased(Input *input, const MouseCode code)
{
    return !input->m_mousemap[code] && input->m_prevMousemap[code];
}

Vec2 InputLastMousePos(Input *input)
{
    return input->m_mousePosition;
}

void InputTypePortal(Input *input, char charPush)
{
    if (input->textLen > 10)
        return;
    input->text[input->textLen] = charPush;
    input->textLen++;
    input->text[input->textLen] = ' ';
}

char *InputGetPortalContent(Input *input)
{
    return input->text;
}

void InputPortalBackspace(Input *input)
{
    input->text[input->textLen - 1] = ' ';
    if (input->textLen > 1)
        input->textLen--;
    return;
}