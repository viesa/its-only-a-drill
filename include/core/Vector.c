#include "Vector.h"

#include "Library.h"

Vector *VectorCreate(size_t elementSize, size_t initialReservedSize)
{
    Vector *vector = MALLOC(Vector);
    ALLOC_ERROR_CHECK(vector);
    vector->data = SDL_malloc(elementSize * initialReservedSize);
    ALLOC_ERROR_CHECK(vector->data);
    vector->dataP = &vector->data;
    vector->capacity = initialReservedSize;
    vector->size = 0ull;
    vector->elementSize = elementSize;
    vector->initialReservedSize = initialReservedSize;
    vector->resizable = SDL_TRUE;

#ifdef VECTOR_DEBUG
    log_info("Vector created. Capacity: %llu E-size: %llu", vector->capacity, vector->elementSize);
#endif
    return vector;
}

void VectorDestroy(Vector *vector)
{
    if (vector)
        FREE(vector->data);
    FREE(vector);
}

VectorP VectorSecureDataPointer(Vector *vector)
{
    return (VectorP)vector->dataP;
}

void VectorResize(Vector *vector, size_t newSize)
{
    vector->capacity = newSize;
    vector->data = SDL_realloc(vector->data, vector->capacity * vector->elementSize);
    ALLOC_ERROR_CHECK(vector->data);
    vector->dataP = &vector->data;
}

void VectorPushBack(Vector *vector, void *element)
{
    assert(element != NULL);
    if (vector->size == vector->capacity)
    {
        if (!vector->resizable)
        {
#ifdef VECTOR_DEBUG
            log_info("Vector is not resizeable");
#endif
            return;
        }
        vector->capacity *= 2ull;
        vector->data = SDL_realloc(vector->data, vector->capacity * vector->elementSize);
        ALLOC_ERROR_CHECK(vector->data);
        vector->dataP = &vector->data;
#ifdef VECTOR_DEBUG
        log_info("Vector capacity increased to %llu", vector->capacity);
#endif
    }
    const size_t dataOffset = vector->size * vector->elementSize;
    SDL_memcpy((char *)vector->data + dataOffset, element, vector->elementSize);
    vector->size++;
#ifdef VECTOR_DEBUG_STRICT
    log_info("Vector element added. New size: %llu", vector->size);
#endif
}

void VectorPopBack(Vector *vector)
{
    if (vector->size == 0)
        return;
    vector->size--;
}

void VectorErase(Vector *vector, size_t index)
{
    VectorEraseRange(vector, index, index + 1);
}

void VectorEraseRange(Vector *vector, size_t start, size_t end)
{
    assert(start <= end);
    const size_t delta = end - start;
    if (!delta)
        return;

    if (start < 0 || start >= vector->size)
    {
#ifdef VECTOR_DEBUG
        log_info("Attempted to remove a vector element out of scope, start");
#endif
        return;
    }
    if (end < 0 || end > vector->size)
    {
#ifdef VECTOR_DEBUG
        log_info("Attempted to remove a vector element out of scope, end");
#endif
        return;
    }

    size_t bytesLeft = (vector->size - end) * vector->elementSize;
    if (bytesLeft)
    {
        size_t dataOffsetStart = start * vector->elementSize;
        size_t dataOffsetEnd = end * vector->elementSize;
        SDL_memmove((char *)(vector->data) + dataOffsetStart, (char *)(vector->data) + dataOffsetEnd, bytesLeft);
#ifdef VECTOR_DEBUG_STRICT
        log_info("Vector elements moved. Bytes moved: %llu", delta, bytesLeft);
#endif
    }
    vector->size -= delta;
#ifdef VECTOR_DEBUG_STRICT
    log_info("%d Vector elements removed. New size: %llu", delta, vector->size);
#endif
}

void VectorEraseElement(Vector *vector, void *element)
{
    for (size_t i = 0; i < vector->size; i++)
    {
        if (!SDL_memcmp((char *)vector->data + i * vector->elementSize, element, vector->elementSize))
        {
            VectorErase(vector, i);
            return;
        }
    }
}

void VectorClear(Vector *vector)
{
#ifdef VECTOR_DEBUG_STRICT
    size_t oldSize = vector->size;
#endif
    if (vector->capacity > vector->initialReservedSize)
    {
        SDL_free(vector->data);
        vector->data = SDL_malloc(vector->elementSize * vector->initialReservedSize);
        ALLOC_ERROR_CHECK(vector->data);
        vector->dataP = &vector->data;
        vector->capacity = vector->initialReservedSize;
    }
    vector->size = 0ull;
#ifdef VECTOR_DEBUG_STRICT
    if (oldSize != 0)
        log_info("Vector cleared, %d elements removed", oldSize);
#endif
}

size_t VectorFind(Vector *vector, void *element)
{
    for (size_t i = 0; i < vector->size; i++)
    {
        if (!SDL_memcmp(&((char *)vector->data)[i], element, vector->elementSize))
        {
            return i;
        }
    }
    // If element was not found, return index = size
    return vector->size;
}

void VectorCopy(Vector *dst, Vector *src)
{
    assert("Trying to copy vectors of different type" && dst->elementSize == src->elementSize);

    SDL_memset(dst->data, 0, dst->capacity);
    VectorResize(dst, src->capacity);
    SDL_memcpy(dst, src, src->capacity);
}