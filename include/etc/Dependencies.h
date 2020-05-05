#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <inttypes.h>
#include <assert.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>

#include "Macros.h"
#include "Log.h"

#define MALLOC(type) (type *)SDL_malloc(sizeof(type))
#define MALLOC_N(type, n) (type *)SDL_malloc(sizeof(type) * (n))

#define CALLOC(nblocks, type) (type *)SDL_calloc((nblocks), sizeof(type))

#define FREE(pointer)      \
    if ((pointer))         \
    {                      \
        SDL_free(pointer); \
    }

#define ALLOC_ERROR_CHECK(pointer)              \
    if ((void *)(pointer) == NULL)              \
    {                                           \
        log_error("Failed to allocate memory"); \
    }

#define MAX_PLAYERNAME_SIZE 20

#endif