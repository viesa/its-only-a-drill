#include "Vector.h"

Vector *VectorCreate(size_t elementSize, size_t initialReservedSize)
{
    Vector *vector = MALLOC(Vector);
    ALLOC_ERROR_CHECK(vector->data);
    vector->data = SDL_malloc(elementSize * initialReservedSize);
    ALLOC_ERROR_CHECK(vector->data);
    vector->dataP = &vector->data;
    vector->capacity = initialReservedSize;
    vector->size = 0ull;
    vector->elementSize = elementSize;

#ifdef VECTOR_DEBUG
    log_info("Vector created. Capacity: %llu E-size: %llu", vector->capacity, vector->elementSize);
#endif
    return vector;
}

void VectorDestroy(Vector *vector)
{
    if (vector->data)
        SDL_free(vector->data);
    if (vector->data)
        SDL_free(vector);
}

VectorP VectorSecureDataPointer(Vector *vector)
{
    return (VectorP)vector->dataP;
}

void VectorPushBack(Vector *vector, void *element)
{
    assert(element != NULL);
    if (vector->size == vector->capacity)
    {
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
    if (index < 0 || index >= vector->size)
    {
#ifdef VECTOR_DEBUG
        log_info("Attempted to remove a vector element out of scope");
#endif
        return;
    }

    size_t bytesLeft = (vector->size - (index + 1)) * vector->elementSize;
    if (bytesLeft)
    {
        size_t dataOffset = index * vector->elementSize;
        SDL_memmove((char *)(vector->data) + dataOffset, (char *)(vector->data) + dataOffset + vector->elementSize, bytesLeft);
#ifdef VECTOR_DEBUG_STRICT
        log_info("Vector elements moved. Bytes moved: %llu", bytesLeft);
#endif
    }
    vector->size--;
#ifdef VECTOR_DEBUG_STRICT
    log_info("Vector element removed. New size: %llu", vector->size);
#endif
}