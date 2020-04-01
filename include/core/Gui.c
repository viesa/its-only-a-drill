#include "Gui.h"
#include "Clock.h"
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define PI 3.14159265359

Gui *GuiCreate(Font *font, Clock *clock)
{
    Gui *gui = (Gui *)SDL_malloc(sizeof(Gui));
    gui->clock = clock;
    gui->font = font;
    gui->points = 3;
    gui->loopCount = 0;
    gui->loopSwing = 87;
    gui->swingDir = 0;
    gui->fps = 0;

    gui->defaultEdge = 30;
    gui->defaultSize = 50;
    gui->defaultOffset = 3;
    srand(time(NULL));

    return gui;
}

void GuiUpdate(Gui *gui)
{
    // Update window size
    int wW = gui->font->gfx->gfxWindowWidth;
    int wH = gui->font->gfx->gfxWindowHeight;

    //if (rand() % 30 < 3)
    //    gui->points += rand() % 500;
    //
    //if (gui->points > 50000)
    //    gui->points = 0;
    gui->points = ScoreInfo(0); //Temp Score grej

    if (gui->loopCount < 2 * PI)
    {
        gui->loopCount += .1f;
    }
    else
    {
        gui->loopCount = 0;
    }

    if (gui->loopSwing >= 255)
    {
        gui->swingDir = 0; // go back down
    }

    if (gui->loopSwing <= 200)
    {
        gui->swingDir = 1; //go back up
    }

    if (gui->swingDir)
    {
        gui->loopSwing++;
    }
    else
    {
        gui->loopSwing--;
    }

    // Points
    char pts[10];
    sprintf(pts, "%ld pts", gui->points);
    SDL_Color vitalsColor[10] = {
        {gui->loopSwing, 159, 227},
        {gui->loopSwing, 139, 207},
        {gui->loopSwing, 119, 187},
        {gui->loopSwing, 99, 167},
        {gui->loopSwing, 79, 147},
        {gui->loopSwing, 59, 127},
        {gui->loopSwing, 39, 107},
        {gui->loopSwing, 19, 87},
        {255 - gui->loopSwing, 180, 184},
        {255 - gui->loopSwing, 180, 184}};

    FontDraw3DCustom(gui->font, TTF_Antilles, pts, wW - gui->defaultEdge, gui->defaultEdge, FAL_R, 0, cos(gui->loopCount) * 1.5, sin(gui->loopCount), 10, vitalsColor); //83

    // Objective
    SDL_Color objColor[2] = {
        {102 + cos(gui->loopCount) * 5, 16, 9},
        {239 + sin(gui->loopCount) * 5, 193, 92}};
    FontDraw3D(gui->font, TTF_Robot_Crush, "The target is a briefcase.", wW / 2, wH - (gui->defaultEdge + 2 * gui->defaultSize), FAL_C, 0, gui->defaultOffset, F3D_TC, 2, objColor);
    FontDraw3D(gui->font, TTF_Robot_Crush, "Discretion is of essence.", wW / 2, wH - (gui->defaultEdge + gui->defaultSize), FAL_C, 0, gui->defaultOffset, F3D_TC, 2, objColor);

    if (!gui->loopCount % 5)
    {
        gui->fps = (int)ClockGetFPS(gui->clock);
    }
    // FPS
    char fps[10];
    sprintf(fps, "%d FPS", gui->fps);
    FontDraw(gui->font, TTF_Arial, fps, 5, 5, FAL_L, 0, (SDL_Color){255, 255, 255}); //83
}

void GuiDestroy(Gui *gui)
{
    SDL_free(gui);
}