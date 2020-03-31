#include "Input.h"

#include "Log.h"

struct Input
{
    SDL_bool m_keymap[SDL_NUM_SCANCODES];
    SDL_bool m_prevKeymap[SDL_NUM_SCANCODES];
};

Input *InputCreate()
{
    Input *ret = (Input *)SDL_malloc(sizeof(Input));
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        ret->m_keymap[i] = SDL_FALSE;
        ret->m_prevKeymap[i] = SDL_FALSE;
    }
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
}

void InputKeyDown(Input *input, const SDL_Scancode key)
{
    input->m_keymap[key] = SDL_TRUE;
}

void InputKeyUp(Input *input, const SDL_Scancode key)
{
    input->m_keymap[key] = SDL_FALSE;
}

SDL_bool InputIsKeyDown(Input *input, const SDL_Scancode key)
{
    return input->m_keymap[key];
}

SDL_bool InputIsKeyPressed(Input *input, const SDL_Scancode key)
{
    return InputIsKeyDown(input, key) && !InputWasKeyDown(input, key);
}

SDL_bool InputWasKeyDown(Input *input, const SDL_Scancode key)
{
    return input->m_prevKeymap[key];
}