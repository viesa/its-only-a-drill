#include "Gui.h"

#include "Library.h"

Gui *GuiCreate(Font *font)
{
    Gui *gui = MALLOC(Gui);
    gui->font = font;
    gui->points = 3;
    gui->loopCount = 0;
    gui->loopSwing = 87;
    gui->swingDir = 0;
    gui->fps = 0;

    gui->defaultEdge = 30;
    gui->defaultSize = 50;
    gui->defaultOffset = 3;
    gui->scan = DrawableCreate((SDL_Rect){0, 0, 1600, 1200}, (SDL_Rect){0, 0, font->gfx->window->width, font->gfx->window->height}, SS_Scanline);

    return gui;
}

void GuiDestroy(Gui *gui)
{
    SDL_free(gui);
}

void GuiOverlayUpdate(Gui *gui)
{
    //Draw scanlines
    gui->scan.dst.w = gui->font->gfx->window->width;
    gui->scan.dst.h = gui->font->gfx->window->height;
    GraphicsDraw(gui->font->gfx, gui->scan);
}

void GuiUpdate(Gui *gui)
{
    //Draw overlays
    GraphicsDrawGradientY(gui->font->gfx, (SDL_Rect){0, 0, gui->font->gfx->window->width, gui->font->gfx->window->height / 5}, (SDL_Color){0, 0, 0, 255}, (SDL_Color){0, 0, 0, 0});
    GraphicsDrawGradientY(gui->font->gfx, (SDL_Rect){0, gui->font->gfx->window->height / 5 * 4, gui->font->gfx->window->width, gui->font->gfx->window->height / 5}, (SDL_Color){0, 0, 0, 0}, (SDL_Color){0, 0, 0, 255});
    GraphicsDrawGradientX(gui->font->gfx, (SDL_Rect){0, 0, gui->font->gfx->window->width / 5, gui->font->gfx->window->height}, (SDL_Color){0, 0, 0, 255}, (SDL_Color){0, 0, 0, 0});
    GraphicsDrawGradientX(gui->font->gfx, (SDL_Rect){gui->font->gfx->window->width / 5 * 4, 0, gui->font->gfx->window->width / 5, gui->font->gfx->window->height}, (SDL_Color){0, 0, 0, 0}, (SDL_Color){0, 0, 0, 255});

    //Points
    gui->points = ScoreInfo(0); //Temp Score grej

    //Update loop variables
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

    // Disp. Points
    char pts[10];
    sprintf(pts, "%ld pts", gui->points);
    SDL_Color vitalsColor[10] = {
        {gui->loopSwing, 159, 227, 255},
        {gui->loopSwing, 139, 207, 255},
        {gui->loopSwing, 119, 187, 255},
        {gui->loopSwing, 99, 167, 255},
        {gui->loopSwing, 79, 147, 255},
        {gui->loopSwing, 59, 127, 255},
        {gui->loopSwing, 39, 107, 255},
        {gui->loopSwing, 19, 87, 255},
        {255 - gui->loopSwing, 180, 184, 255},
        {255 - gui->loopSwing, 180, 184, 255}};

    FontDraw3DCustom(gui->font, FontGetDynamicSizing(gui->font), pts, gui->font->gfx->window->width - gui->defaultEdge, gui->defaultEdge, FC_ALIGN_RIGHT, 0, cos(gui->loopCount) * 1.5, sin(gui->loopCount), 10, vitalsColor); //83

    // Disp. Objective
    //SDL_Color objColor[2] = {
    //{102 + cos(gui->loopCount) * 5, 16, 9},
    //{239 + sin(gui->loopCount) * 5, 193, 92}};
    //FontDraw3D(gui->font, TTF_Robot_Crush, "The target is a briefcase.", wW / 2, wH - (gui->defaultEdge + 2 * gui->defaultSize), FC_ALIGN_CENTER, 0, gui->defaultOffset, F3D_TC, 2, objColor);
    //FontDraw3D(gui->font, TTF_Robot_Crush, "Discretion is of essence.", wW / 2, wH - (gui->defaultEdge + gui->defaultSize), FC_ALIGN_CENTER, 0, gui->defaultOffset, F3D_TC, 2, objColor);

    GuiOverlayUpdate(gui);
}

void GuiDrawFPS(Gui *gui)
{
    if (!gui->loopCount % 5)
    {
        gui->fps = (int)ClockGetFPS();
    }
    char fps[10];
    sprintf(fps, "%d", gui->fps);
    FontDraw(gui->font, TTF_Arial, fps, 5, 5, FC_ALIGN_LEFT, 0, (SDL_Color){255, 255, 255}); //83
}