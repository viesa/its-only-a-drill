#ifndef INPUT_H
#define INPUT_H

#include "Dependencies.h"
#include "Keybinding.h"

#include "Vec2.h"

typedef enum MouseCode
{
    BUTTON_LEFT = 1,
    BUTTON_MIDDLE = 2,
    BUTTON_RIGHT = 3,
    BUTTON_X1 = 4,
    BUTTON_X2 = 5
} MouseCode;

void InputInitialize();
void InputUninitialize();

/// Copies keymap of this frame the
/// previous frame's keymap
void InputUpdateKeymaps();

/// Updates keymap for this frame
void InputKeyDown(const SDL_Scancode key);
/// Updates keymap for this frame
void InputKeyUp(const SDL_Scancode key);
/// Updates mousemap for this frame
void InputMouseDown(const MouseCode code);
/// Updates mousemap for this frame
void InputMouseUp(const MouseCode code);
/// Updates mouse position
void InputMouseMove(Vec2 pos);
/// Returns true if key is down this frame
SDL_bool InputIsKeyDown(const SDL_Scancode key);
/// Returns true if key is down this frame,
/// and down in the previous frame
SDL_bool InputIsKeyPressed(const SDL_Scancode key);
/// Returns true if key is up this frame,
/// but was in the previous frame
SDL_bool InputIsKeyReleased(const SDL_Scancode key);
// Returns true if any key is down this frame
SDL_bool InputIsAnyKeyDown();

/// Returns true if mouse is down this frame
SDL_bool InputIsMouseDown(const MouseCode code);
/// Returns true if mouse is down this frame,
/// but not in the previous frame
SDL_bool InputIsMousePressed(const MouseCode code);
/// Returns true if mouse is up this frame,
/// and down in the previous frame
SDL_bool InputIsMouseReleased(const MouseCode code);
/// Returns the last captured mouse position
Vec2 InputLastMousePos();

/// Reruens the last pressed key
SDL_Scancode InputLastKeyDown(SDL_Scancode normal);

void InputTypePortal(char charPush);
char *InputGetPortalContent();
void InputClearPortalContent();
void InputPortalBackspace();
#endif