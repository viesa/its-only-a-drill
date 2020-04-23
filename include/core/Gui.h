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
    Clock *clock;
    long int points;
    float loopCount;
    float loopSwing;
    float swingDir;
    int fps;
    float defaultEdge;
    float defaultSize;
    float defaultOffset;
} Gui;

typedef enum GuiWindow
{
    GUI_Vitals,
    GUI_Count
} GuiWindow;

Gui *GuiCreate(Font *font, Clock *clock);
void GuiUpdate(Gui *gui);
void GuiDestroy(Gui *gui);

#endif