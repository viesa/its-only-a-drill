#ifndef INPUT_H
#define INPUT_H
#include <SDL2/SDL_bits.h>

typedef enum Code
{
    EVENT_QUIT,
    MOUSE_L,
    MOUSE_R,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_ESC,
    CODE_nCodes
} Code;

typedef struct Input Input;

Input *InputCreate();
void InputDestroy(Input *input);

void InputReset(Input *input);

SDL_bool InputGet(Input *input, const Code key);
void InputSet(Input *input, const Code key, SDL_bool newState);
#endif