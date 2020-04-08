#include "State.h"

State StateCreate()
{
    State state;
    state.gameState = GS_None;
    state.menuState = MS_None;
    return state;
}