#ifndef GUI_H
#define GUI_H

#include "Dependencies.h"
#include "Font.h"
#include "Clock.h"
#include "Score.h"

typedef enum GuiWindow
{
    GUI_Vitals,
    GUI_Count
} GuiWindow;

void GuiInitialize();
void GuiUninitialize();

void GuiUpdate();
void GuiOverlayUpdate();
void GuiDrawFPS();

#endif