#include "JSON.h"

static void process_value_find(json_value *value, json_type type, void *data, int size, json_value **result);
static void process_value(json_value *value, int depth);

JSON *JSONCreate(char *filename)
{

    JSON *json = MALLOC(JSON);

    FileIO lfile = FileIOCreate(filename);
    if (!FileIORead(&lfile))
        return NULL;
    json->file_contents = MALLOC_N(json_char, lfile.size);
    memcpy(json->file_contents, lfile.contents, lfile.size);
    json->value = json_parse(json->file_contents, lfile.size);

    FileIODestroy(&lfile);

    if (json->value == NULL)
    {
        log_error("JSON Loading error: Unable to parse data");
        FREE(json);
        return NULL;
    }

    json->originalFilename = MALLOC_N(char, strlen(filename));
    ALLOC_ERROR_CHECK(json->originalFilename);
    strcpy(json->originalFilename, filename);

    return json;
}

JSON *JSONCreateFromArray(char *loadedfile, size_t size)
{
    JSON *json = MALLOC(JSON);

    json->file_contents = MALLOC_N(json_char, size);
    memcpy(json->file_contents, loadedfile, size);
    json->value = json_parse(json->file_contents, size);

    if (json->value == NULL)
    {
        log_error("JSON Loading error: Unable to parse data");
        FREE(json);
        return NULL;
    }

    char *substitute = "Unknown";
    json->originalFilename = MALLOC_N(char, strlen(substitute));
    ALLOC_ERROR_CHECK(json->originalFilename);
    strcpy(json->originalFilename, substitute);

    return json;
}

void JSONDestroy(JSON *json)
{
    json_value_free(json->value);
    if (json)
    {
        FREE(json->file_contents);
        FREE(json->originalFilename);
    }
    FREE(json);
}

void JSONPrint(JSON *json)
{
    process_value(json->value, 0);
}

json_value *JSONGetValue(JSON *json, uint32_t indices[], uint32_t n)
{
    json_value *ret = json->value;
    for (int i = 0; i < n; i++)
    {
        json_type type = ret->type;
        uint32_t currIndex = indices[i];
        if (type == json_object)
        {
            uint32_t length = ret->u.object.length;
            if (length <= currIndex)
                return NULL;
            ret = ret->u.object.values[currIndex].value;
        }
        else if (type == json_array)
        {
            uint32_t length = ret->u.array.length;
            if (length <= currIndex)
                return NULL;
            ret = ret->u.array.values[currIndex];
        }
        else
            return NULL;
    }
    return ret;
}

json_value *JSONFind(JSON *json, json_value *start, json_type type, void *data, int size)
{
    if (!start)
    {
        start = json->value;
    }
    json_value *ret = NULL;
    process_value_find(start, type, data, size, &ret);
    return ret;
}

// Helper functions for finding objects
static void process_object_find(json_value *value, json_type type, void *data, int size, json_value **result)
{
    if (value == NULL)
    {
        return;
    }
    int length = value->u.object.length;
    for (int i = 0; i < length; i++)
    {
        if (type == value->u.object.values[i].value->type &&
            value->u.object.values[i].name_length == size &&
            !SDL_memcmp(value->u.object.values[i].name, data, size))
        {
            *result = value->u.object.values[i].value;
            return;
        }
        process_value_find(value->u.object.values[i].value, type, data, size, result);
        if (*result != NULL)
            return;
    }
}

static void process_array_find(json_value *value, json_type type, void *data, int size, json_value **result)
{
    if (value == NULL)
    {
        return;
    }
    int length = value->u.array.length;
    for (int i = 0; i < length; i++)
    {
        process_value_find(value->u.array.values[i], type, data, size, result);
        if (*result != NULL)
            return;
    }
}

static void process_value_find(json_value *value, json_type type, void *data, int size, json_value **result)
{
    if (value == NULL)
    {
        return;
    }

    switch (value->type)
    {
    case json_none:
    {
        break;
    }
    case json_object:
    {
        process_object_find(value, type, data, size, result);
        break;
    }
    case json_array:
    {
        if (type == value->type &&
            size == sizeof(int) &&
            *(int *)data == value->u.array.length)
        {
            *result = value;
            return;
        }
        process_array_find(value, type, data, size, result);
        break;
    }
    default:
        break;
    }
}

// ------- HELPER FUNCTIONS FOR PRINTING --------

static void print_depth_shift(int depth)
{
    int j;
    for (j = 0; j < depth; j++)
    {
        printf(" ");
    }
}

static void process_object(json_value *value, int depth)
{
    int length, x;
    if (value == NULL)
    {
        return;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++)
    {
        print_depth_shift(depth);
        printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
        process_value(value->u.object.values[x].value, depth + 1);
    }
}

static void process_array(json_value *value, int depth)
{
    int length, x;
    if (value == NULL)
    {
        return;
    }
    length = value->u.array.length;
    printf("array\n");
    for (x = 0; x < length; x++)
    {
        process_value(value->u.array.values[x], depth);
    }
}

static void process_value(json_value *value, int depth)
{
    if (value == NULL)
    {
        return;
    }
    if (value->type != json_object)
    {
        print_depth_shift(depth);
    }
    switch (value->type)
    {
    case json_none:
        printf("none\n");
        break;
    case json_object:
        process_object(value, depth + 1);
        break;
    case json_array:
        process_array(value, depth + 1);
        break;
    case json_integer:
        printf("int: %10" PRId64 "\n", value->u.integer);
        break;
    case json_double:
        printf("double: %f\n", value->u.dbl);
        break;
    case json_string:
        printf("string: %s\n", value->u.string.ptr);
        break;
    case json_boolean:
        printf("bool: %d\n", value->u.boolean);
        break;
    default:
        break;
    }
}