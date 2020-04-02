#ifndef INPUT_H
#define INPUT_H
#include <SDL2/SDL_bits.h>
#include <SDL2/SDL_keyboard.h>

typedef struct Input Input;

Input *InputCreate();
void InputDestroy(Input *input);

/// Copies keymap of this frame the
/// previous frame's keymap
void InputUpdate(Input *input);

/// Updates keymap for this frame
void InputKeyDown(Input *input, const SDL_Scancode key);
/// Updates keymap for this frame
void InputKeyUp(Input *input, const SDL_Scancode key);

/// Returns true if key is down this frame
SDL_bool InputIsKeyDown(Input *input, const SDL_Scancode key);
/// Returns true if key is down this frame,
/// but not in the previous frame
SDL_bool InputIsKeyPressed(Input *input, const SDL_Scancode key);

/// Returns true if key was down previous frame
SDL_bool InputWasKeyDown(Input *input, const SDL_Scancode key);

#endif