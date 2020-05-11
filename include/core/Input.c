#include "Input.h"

#define NUM_MOUSE_EVENTS 6

struct Input
{
    SDL_bool *keymap;
    SDL_bool *prevKeymap;

    SDL_bool *mousemap;
    SDL_bool *prevMousemap;

    Vec2 mousePosition;
    int textLen;
    char *text;
} input;

void InputInitialize()
{

    input.keymap = MALLOC_N(SDL_bool, SDL_NUM_SCANCODES);
    input.prevKeymap = MALLOC_N(SDL_bool, SDL_NUM_SCANCODES);

    input.mousemap = MALLOC_N(SDL_bool, NUM_MOUSE_EVENTS);
    input.prevMousemap = MALLOC_N(SDL_bool, NUM_MOUSE_EVENTS);

    input.text = MALLOC_N(char, 16);

    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        input.keymap[i] = SDL_FALSE;
        input.prevKeymap[i] = SDL_FALSE;
    }
    for (int i = 0; i < NUM_MOUSE_EVENTS; i++)
    {
        input.mousemap[i] = SDL_FALSE;
        input.prevMousemap[i] = SDL_FALSE;
    }

    input.textLen = 0;
    SDL_memset(input.text, 0, 16);
}
void InputUninitialize()
{
    FREE(input.keymap);
    FREE(input.prevKeymap);
    FREE(input.mousemap);
    FREE(input.prevMousemap);
    FREE(input.text);
}

void InputUpdateKeymaps()
{
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        input.prevKeymap[i] = input.keymap[i];
    }
    for (int i = 0; i < NUM_MOUSE_EVENTS; i++)
    {
        input.prevMousemap[i] = input.mousemap[i];
    }
}

void InputKeyDown(const SDL_Scancode key)
{
    input.keymap[key] = SDL_TRUE;
}

void InputKeyUp(const SDL_Scancode key)
{
    input.keymap[key] = SDL_FALSE;
}

void InputMouseDown(const MouseCode code)
{
    input.mousemap[code] = SDL_TRUE;
}

void InputMouseUp(const MouseCode code)
{
    input.mousemap[code] = SDL_FALSE;
}

void InputMouseMove(Vec2 pos)
{
    input.mousePosition = pos;
}

SDL_bool InputIsKeyDown(const SDL_Scancode key)
{
    return input.keymap[key];
}

SDL_bool InputIsKeyPressed(const SDL_Scancode key)
{
    return input.keymap[key] && !input.prevKeymap[key];
}

SDL_bool InputIsKeyReleased(const SDL_Scancode key)
{
    return !input.keymap[key] && input.prevKeymap[key];
}

SDL_bool InputIsAnyKeyDown()
{
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        if (input.keymap[i])
            return SDL_TRUE;
    }
    return SDL_FALSE;
}

SDL_bool InputIsMouseDown(const MouseCode code)
{
    return input.mousemap[code];
}

SDL_bool InputIsMousePressed(const MouseCode code)
{
    return input.mousemap[code] && !input.prevMousemap[code];
}

SDL_bool InputIsMouseReleased(const MouseCode code)
{
    return !input.mousemap[code] && input.prevMousemap[code];
}

Vec2 InputLastMousePos()
{
    return input.mousePosition;
}

void InputTypePortal(char charPush)
{
    if (charPush < 32 && charPush > 126)
        return;
    if (input.textLen > 12)
        return;
    input.text[input.textLen] = charPush;
    input.textLen++;
    input.text[input.textLen] = '\0';
}

char *InputGetPortalContent()
{
    return input.text;
}

void InputClearPortalContent()
{
    input.textLen = 0;
    for (int i = 0; i < 12; i++)
    {
        input.text[i] = '\0';
    }
}

void InputPortalBackspace()
{
    input.text[input.textLen - 1] = '\0';
    if (input.textLen > 0)
        input.textLen--;
    return;
}

SDL_Scancode InputLastKeyDown(SDL_Scancode normal)
{
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        if (input.keymap[i])
        {
            if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
            {
            }
            else
            {
                return (SDL_Scancode)i;
            }
        }
    }
    return normal;
}