#ifndef CURSOR_H
#define CURSOR_H

#include "Dependencies.h"

typedef enum CursorType
{
    CT_Normal,
    CT_Crossair,
    CT_None,
    CT_Count
} CursorType;

void CursorInitialize();
void CursorChange(CursorType type);

#endif