#ifndef GUI_H
#define GUI_H

#include "Dependencies.h"
#include "Font.h"
#include "Clock.h"
#include "Score.h"

//Graphical User Interface - GUI
typedef struct Gui
{
    Font *font;
    long int points;
    float loopCount;
    float loopSwing;
    float swingDir;
    int fps;
    float defaultEdge;
    float defaultSize;
    float defaultOffset;
    Drawable scan;
} Gui;

typedef enum GuiWindow
{
    GUI_Vitals,
    GUI_Count
} GuiWindow;

Gui *GuiCreate(Font *font);
void GuiDestroy(Gui *gui);

void GuiUpdate(Gui *gui);
void GuiOverlayUpdate(Gui *gui);
void GuiDrawFPS(Gui *gui);

#endif