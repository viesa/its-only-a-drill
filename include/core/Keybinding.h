#ifndef KEYBINDING_H
#define KEYBINDING_H
#define BINDINGS 16

#include "Input.h"

#define KEYBINDINGS_KEYS KeybindingGetKeys()

typedef enum ActionCode
{
    AC_MOVE_UP = 0,
    AC_MOVE_LEFT = 1,
    AC_MOVE_RIGHT = 2,
    AC_MOVE_DOWN = 3,
    AC_PICKUP = 4,
    AC_DROP = 5,
    AC_SHOOT = 6,
    AC_RELOAD = 7,
    AC_INVENTORY = 8,
    AC_Count = 9
} ActionCode;

void KeybindingInitialize();
void KeybindingUninitialize();

void KeybindingChange(ActionCode action, SDL_Scancode key);

SDL_Scancode *KeybindingGetKeys();

#endif