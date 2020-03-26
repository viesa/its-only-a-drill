#include "Gui.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

Gui *GuiCreate(Font *font)
{
    Gui *gui = (Gui *)SDL_malloc(sizeof(gui));
    gui->font = font;
    gui->points = 3;
    srand(time(NULL));

    return gui;
}

void GuiLoop(Gui *gui)
{
    //Gui vars
    int wW = gui->font->gfx->gfxWindowWidth;
    int wH = gui->font->gfx->gfxWindowHeight;
    int edge = 30;
    int size = 50;
    int offset3d = 3;

    if (rand() % 30 < 3)
        gui->points += rand() % 500;

    if (gui->points > 50000)
        gui->points = 0;

    char pts[10];

    sprintf(pts, "%d pts", gui->points);

    // Vitals
    SDL_Rect pointsSize = FontGetSize(gui->font, TTF_Robot_Crush, pts);
    FontDraw3D(gui->font, TTF_Robot_Crush, pts, wW - pointsSize.w - edge, edge, 3, (SDL_Color){186, 39, 107}, (SDL_Color){87, 180, 184});

    // Objective

    SDL_Rect objLine1 = FontGetSize(gui->font, TTF_Robot_Crush, "The target is a briefcase.");
    FontDraw3D(gui->font, TTF_Robot_Crush, "The target is a briefcase.", wW / 2 - objLine1.w / 2, wH - (edge + 2 * size), offset3d, (SDL_Color){102, 16, 9}, (SDL_Color){239, 193, 92});

    SDL_Rect objLine2 = FontGetSize(gui->font, TTF_Robot_Crush, "Discretion is of essence.");
    FontDraw3D(gui->font, TTF_Robot_Crush, "Discretion is of essence.", wW / 2 - objLine2.w / 2, wH - (edge + size), offset3d, (SDL_Color){102, 16, 9}, (SDL_Color){239, 193, 92});
}