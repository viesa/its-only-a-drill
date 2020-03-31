#ifndef INPUT_H
#define INPUT_H
#include <SDL2/SDL_bits.h>
#include <SDL2/SDL_keyboard.h>

typedef struct Input Input;

Input *InputCreate();
void InputDestroy(Input *input);

void InputUpdate(Input *input);

void InputKeyDown(Input *input, const SDL_Scancode key);
void InputKeyUp(Input *input, const SDL_Scancode key);

SDL_bool InputIsKeyDown(Input *input, const SDL_Scancode key);
SDL_bool InputIsKeyPressed(Input *input, const SDL_Scancode key);

SDL_bool InputWasKeyDown(Input *input, const SDL_Scancode key);

#endif