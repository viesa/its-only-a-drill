#ifndef GUI_H
#define GUI_H

#include "Font.h"
#include "Clock.h"

typedef struct Gui
{
    Font *font;
    Clock *clock;
    long int points;
    int loopCount;
    int loopSwing;
    int swingDir;
} Gui;

typedef enum GuiWindow
{
    GUI_Vitals,
    GUI_Count
} GuiWindow;

Gui *GuiCreate(Font *font, Clock *clock);
void GuiUpdate(Gui *gui);

#endif