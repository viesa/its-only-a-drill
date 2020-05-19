#include "Keybinding.h"

typedef struct Keybinding
{
    SDL_Scancode KeyArray[BINDINGS];
} Keybinding;

static Keybinding *bindings;

void KeybindingInitialize()
{
    bindings = MALLOC(Keybinding);
    ALLOC_ERROR_CHECK(bindings);

    bindings->KeyArray[AC_MOVE_UP] = SDL_SCANCODE_W;
    bindings->KeyArray[AC_MOVE_LEFT] = SDL_SCANCODE_A;
    bindings->KeyArray[AC_MOVE_RIGHT] = SDL_SCANCODE_D;
    bindings->KeyArray[AC_MOVE_DOWN] = SDL_SCANCODE_S;
    bindings->KeyArray[AC_PICKUP] = SDL_SCANCODE_Q;
    bindings->KeyArray[AC_DROP] = SDL_SCANCODE_Z;
    bindings->KeyArray[AC_SHOOT] = (SDL_Scancode)BUTTON_LEFT;
    bindings->KeyArray[AC_RELOAD] = SDL_SCANCODE_R;
    bindings->KeyArray[AC_INVENTORY] = SDL_SCANCODE_TAB;
}

void KeybindingUninitialize()
{
    FREE(bindings);
}

void KeybindingChange(ActionCode action, SDL_Scancode key)
{
    bindings->KeyArray[action] = key;
}

SDL_Scancode *KeybindingGetKeys()
{
    return bindings->KeyArray;
}