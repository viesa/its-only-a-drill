#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <sys/stat.h>
#include <SDL2/SDL_bits.h>

#include "JSONParser.h"
#include "Log.h"

typedef struct JSON
{
    json_char *file_contents;
    json_value *value;
} JSON;

JSON *JSONCreate(char *filename);
void JSONDestroy(JSON *json);

void JSONPrint(JSON *json);
json_value *JSONGetValue(JSON *json, uint32_t indices[], uint32_t n);

#endif