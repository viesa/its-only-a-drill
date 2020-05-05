#include "Cursor.h"

struct
{
    CursorType type;
    SDL_Cursor *sdl_cursor;
} U_CURSOR;

void CursorInitialize()
{
    U_CURSOR.type = CT_None;
    U_CURSOR.sdl_cursor = NULL;
}

void CursorChange(CursorType type)
{
    if (U_CURSOR.type == type)
        return;
    else
        U_CURSOR.type = type;

    if (U_CURSOR.sdl_cursor)
        SDL_FreeCursor(U_CURSOR.sdl_cursor);

    SDL_Surface *cursorSurface = NULL;
    switch (type)
    {
    case CT_Normal:
        cursorSurface = SDL_LoadBMP("assets/cursors/cursor.bmp");
        U_CURSOR.sdl_cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
        break;
    case CT_Crossair:
        cursorSurface = SDL_LoadBMP("assets/cursors/crosshair.bmp");
        U_CURSOR.sdl_cursor = SDL_CreateColorCursor(cursorSurface, 15, 15);
        break;
    default:
        break;
    }
    if (U_CURSOR.sdl_cursor)
        SDL_SetCursor(U_CURSOR.sdl_cursor);
    if (cursorSurface)
        SDL_FreeSurface(cursorSurface);
}