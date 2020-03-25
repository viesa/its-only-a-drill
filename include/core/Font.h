#ifndef FONTS_H
#define FONTS_H

#include "Graphics.h"

typedef struct Font Font;

Font *FontsCreate();
void FontsDestroy(Font *font);


#endif