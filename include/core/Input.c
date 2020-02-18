#include "Input.h"

struct Input
{
    SDL_bool m_events[CODE_nCodes];
};

Input *InputCreate()
{
    Input *input_ret = (Input *)SDL_malloc(sizeof(Input));
    InputReset(input_ret);
    return input_ret;
}
void InputDestroy(Input *input)
{
    SDL_free(input);
}

void InputReset(Input *input)
{
    for (int i = 0; i < CODE_nCodes; i++)
    {
        input->m_events[i] = SDL_FALSE;
    }
}

SDL_bool InputGet(Input *input, const Code key)
{
    return input->m_events[key];
}
void InputSet(Input *input, const Code key, SDL_bool newState)
{
    input->m_events[key] = newState;
}