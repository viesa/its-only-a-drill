#include "Cursor.h"

struct
{
    CursorType type;
    SDL_Cursor *sdl_cursor;
} cursor;

void CursorInitialize()
{
    cursor.type = CT_None;
    cursor.sdl_cursor = NULL;
}

void CursorChange(CursorType type)
{
    if (cursor.type == type)
        return;
    else
        cursor.type = type;

    if (cursor.sdl_cursor)
        SDL_FreeCursor(cursor.sdl_cursor);

    SDL_Surface *cursorSurface = NULL;
    switch (type)
    {
    case CT_Normal:
        cursorSurface = SDL_LoadBMP("assets/cursors/cursor.bmp");
        cursor.sdl_cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
        break;
    case CT_Crossair:
        cursorSurface = SDL_LoadBMP("assets/cursors/crosshair.bmp");
        cursor.sdl_cursor = SDL_CreateColorCursor(cursorSurface, 15, 15);
        break;
    default:
        break;
    }
    if (cursor.sdl_cursor)
        SDL_SetCursor(cursor.sdl_cursor);
    if (cursorSurface)
        SDL_FreeSurface(cursorSurface);
}