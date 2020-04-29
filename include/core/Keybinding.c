#include "Keybinding.h"

Keybinding *KeybindingCreate() 
{
    Keybinding *bindings = MALLOC(Keybinding);
    bindings->KeyArray[MOVE_UP] = SDL_SCANCODE_W;
    bindings->KeyArray[MOVE_LEFT] = SDL_SCANCODE_A;
    bindings->KeyArray[MOVE_RIGHT] = SDL_SCANCODE_D;
    bindings->KeyArray[MOVE_DOWN] = SDL_SCANCODE_S;
    bindings->KeyArray[PICKUP] = SDL_SCANCODE_Q;
    bindings->KeyArray[DROP] = SDL_SCANCODE_Z;
    bindings->KeyArray[SHOOT] = BUTTON_LEFT;
    bindings->KeyArray[RELOAD] = SDL_SCANCODE_R;
    bindings->KeyArray[INVENTORY] = SDL_SCANCODE_TAB;
    return bindings;
}

void KeybindingChange(ActionCode action, SDL_Scancode key, Keybinding *bindings) 
{
    bindings->KeyArray[action] = key;
}

void KeybindingFree(Keybinding *bindings) 
{
    SDL_free(bindings);
}

/*void KeybindingChange2(const SDL_Scancode key, SDL_Scancode key) 
{

}*/

