#ifndef KEYBINDING_H
#define KEYBINDING_H
#define BINDINGS 16

#include "Input.h"

typedef enum ActionCode
{
    MOVE_UP = 0,
    MOVE_LEFT = 1,
    MOVE_RIGHT = 2,
    MOVE_DOWN = 3,
    PICKUP = 4,
    DROP = 5,
    SHOOT = 6,
    RELOAD = 7,
    INVENTORY = 8
} ActionCode;

typedef struct Keybinding 
{
    SDL_Scancode KeyArray[BINDINGS];
}Keybinding;

Keybinding *KeybindingCreate(); 
void KeybindingChange(ActionCode action, SDL_Scancode key, Keybinding *bindings);
void KeybindingFree(Keybinding *bindings); 

#endif