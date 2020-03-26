#ifndef GUI_H
#define GUI_H

#include "Font.h"

typedef struct Gui
{
    Font *font;
    long int points;
} Gui;

typedef enum GuiWindow
{
    GUI_Vitals,
    GUI_Count
} GuiWindow;

Gui *GuiCreate(Font *font);
void GuiLoop(Gui *gui);

#endif