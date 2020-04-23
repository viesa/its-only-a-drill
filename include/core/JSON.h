#ifndef JSON_H
#define JSON_H

#include "Dependencies.h"
#include "JSONParser.h"

typedef struct JSON
{
    json_char *file_contents;
    json_value *value;
} JSON;

/// Returns NULL if failed to load JSON-file
JSON *JSONCreate(char *filename);
void JSONDestroy(JSON *json);

/// Formatted print of the JSON data
void JSONPrint(JSON *json);
/// Fetches data from the JSON-file
/// \param indices: How to traverse the JSON data
///     ex. 0,3,2 will traverse the JSON data through
///         the first value, then the 4th and so on.
///     returns early (NULL) if index not found in JSON-object
/// \param n: number of elements in previous parameter
json_value *JSONGetValue(JSON *json, uint32_t indices[], uint32_t n);

#endif