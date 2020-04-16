#ifndef INPUT_H
#define INPUT_H
#include <SDL2/SDL_bits.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>

typedef enum MouseCode
{
    BUTTON_LEFT = 1,
    BUTTON_MIDDLE = 2,
    BUTTON_RIGHT = 3,
    BUTTON_X1 = 4,
    BUTTON_X2 = 5
} MouseCode;

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
/// Updates mousemap for this frame
void InputMouseDown(Input *input, const MouseCode code);
/// Updates mousemap for this frame
void InputMouseUp(Input *input, const MouseCode code);

/// Returns true if key is down this frame
SDL_bool InputIsKeyDown(Input *input, const SDL_Scancode key);
/// Returns true if key is down this frame,
/// and down in the previous frame
SDL_bool InputIsKeyPressed(Input *input, const SDL_Scancode key);
/// Returns true if key is up this frame,
/// but was in the previous frame
SDL_bool InputIsKeyReleased(Input *input, const SDL_Scancode key);

/// Returns true if mouse is down this frame
SDL_bool InputIsMouseDown(Input *input, const MouseCode code);
/// Returns true if mouse is down this frame,
/// but not in the previous frame
SDL_bool InputIsMousePressed(Input *input, const MouseCode code);
/// Returns true if mouse is up this frame,
/// and down in the previous frame
SDL_bool InputIsMouseReleased(Input *input, const MouseCode code);

#endif