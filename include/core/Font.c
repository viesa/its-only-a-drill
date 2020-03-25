#include "Font.h"

#include "Log.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct Font
{
    Graphics *gfx;
    TTF_Font *font;
};

Font *FontsCreate(Graphics *gfx, char filePath[], int size){
    TTF_Init();
    Font font;
    font.gfx = gfx;
    font.font = TTF_OpenFont("../assets/fonts/arial.ttf", 25); //filepath, size
    return &font;
}

void FontsDestroy(Font *font){
    TTF_Quit();
}
