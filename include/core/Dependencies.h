#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>

#include "Macros.h"
#include "Log.h"
#include "Library.h"
#include "../math/Vec2.h"

#define MALLOC(type) (type *)SDL_malloc(sizeof(type))
#define MALLOC_N(type, n) (type *)SDL_malloc(sizeof(type) * (n))

#endif