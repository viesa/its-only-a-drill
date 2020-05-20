#include "Gui.h"

#include "Library.h"

//Graphical User Interface - GUI
typedef struct Gui
{
    long int points;
    float loopCount;
    float loopSwing;
    float swingDir;
    float fps;
    float defaultEdge;
    float defaultSize;
    float defaultOffset;
    Drawable scan;
} Gui;

static Gui *gui;

void GuiInitialize()
{
    gui = MALLOC(Gui);
    ALLOC_ERROR_CHECK(gui);

    gui->points = 3;
    gui->loopCount = 0;
    gui->fps = 0;

    gui->defaultEdge = 30;
    gui->defaultSize = 50;
    gui->defaultOffset = 3;
    gui->scan = DrawableCreate((SDL_Rect){0, 0, 1600, 1200}, (SDL_Rect){0, 0, WindowGetWidth(), WindowGetHeight()}, SS_Scanline);
}

void GuiUninitialize()
{
    FREE(gui);
}

void GuiOverlayUpdate()
{
    //Draw scanlines
    gui->scan.dst.w = WindowGetWidth();
    gui->scan.dst.h = WindowGetHeight();
    GraphicsDraw(gui->scan);
}

void GuiUpdate()
{
    //Draw overlays
    GraphicsDrawGradientY((SDL_Rect){0, 0, WindowGetWidth(), WindowGetHeight() / 5}, (SDL_Color){0, 0, 0, 255}, (SDL_Color){0, 0, 0, 0});
    GraphicsDrawGradientY((SDL_Rect){0, WindowGetHeight() / 5 * 4, WindowGetWidth(), WindowGetHeight() / 5}, (SDL_Color){0, 0, 0, 0}, (SDL_Color){0, 0, 0, 255});
    GraphicsDrawGradientX((SDL_Rect){0, 0, WindowGetWidth() / 5, WindowGetHeight()}, (SDL_Color){0, 0, 0, 255}, (SDL_Color){0, 0, 0, 0});
    GraphicsDrawGradientX((SDL_Rect){WindowGetWidth() / 5 * 4, 0, WindowGetWidth() / 5, WindowGetHeight()}, (SDL_Color){0, 0, 0, 0}, (SDL_Color){0, 0, 0, 255});

    //Points
    gui->points = ScoreboardGetScore(PlayerGetEntity()->id); //Temp Score grej

    //Update loop variables
    if (gui->loopCount < 2 * PI)
    {
        gui->loopCount += ClockGetDeltaTime() * 2 * PI;
    }
    else
    {
        gui->loopCount = 0;
    }

    // Disp. Points

    long points = gui->points;
    char pts[20];
    if (points >= 1000000)
    {
        points = 1000000;
        sprintf(pts, "> %ld pts", points);
    }
    else if (points <= -1000000)
    {
        points = -1000000;
        sprintf(pts, "< %ld pts", points);
    }
    else
    {
        sprintf(pts, "%ld pts", points);
    }
    SDL_Color vitalsColor[10] = {
        {225, 159, 227, 255},
        {225, 139, 207, 255},
        {225, 119, 187, 255},
        {225, 99, 167, 255},
        {225, 79, 147, 255},
        {225, 59, 127, 255},
        {225, 39, 107, 255},
        {225, 19, 87, 255},
        {40, 180, 184, 255},
        {40, 180, 184, 255}};

    FontDraw3DCustom(FontGetDynamicSizing(), pts, WindowGetWidth() - gui->defaultEdge, gui->defaultEdge, FAL_R, 0, cos(gui->loopCount) * 1.5, sin(gui->loopCount), 10, vitalsColor); //83

    // Disp. Objective
    //SDL_Color objColor[2] = {
    //{102 + cos(gui->loopCount) * 5, 16, 9},
    //{239 + sin(gui->loopCount) * 5, 193, 92}};
    //FontDraw3D(gui->font, TTF_Robot_Crush, "The target is a briefcase.", wW / 2, wH - (gui->defaultEdge + 2 * gui->defaultSize), FC_ALIGN_CENTER, 0, gui->defaultOffset, F3D_TC, 2, objColor);
    //FontDraw3D(gui->font, TTF_Robot_Crush, "Discretion is of essence.", wW / 2, wH - (gui->defaultEdge + gui->defaultSize), FC_ALIGN_CENTER, 0, gui->defaultOffset, F3D_TC, 2, objColor);

    GuiOverlayUpdate();
}

void GuiDrawFPS()
{
    if (!gui->loopCount % 5)
    {
        gui->fps = ClockGetFPS();
    }
    char fps[10];
    sprintf(fps, "%.0f", gui->fps);
    FontDraw(TTF_Arial, fps, 5, 5, FAL_L, 0, (SDL_Color){255, 255, 255});
}

void GuiDrawFinishedRoundMessage()
{
    GraphicsDrawRect(WindowGetScreenRect(), (SDL_Color){0, 0, 0, 120}, SDL_TRUE);
    char buffer[100] = {0};
    sprintf(buffer, "Round finished! New round starting. [%.0f]", RoundGetCountdown());
    FontDraw(FontGetDynamicSizing(), buffer, WindowGetWidth() / 2, WindowGetHeight() / 2, FAL_C, 0, (SDL_Color){40, 180, 184, 255});
}

void GuiDrawFinishedMatchMessage()
{
    GraphicsDrawRect(WindowGetScreenRect(), (SDL_Color){0, 0, 0, 120}, SDL_TRUE);
    char buffer[100] = {0};
    sprintf(buffer, "Finished match. [%.0f]", RoundGetCountdown());
    FontDraw(FontGetDynamicSizing(), buffer, WindowGetWidth() / 2, WindowGetHeight() / 2, FAL_C, 0, (SDL_Color){40, 180, 184, 255});
}