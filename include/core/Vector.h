#ifndef VECTOR_H
#define VECTOR_H

#include "Dependencies.h"

typedef struct Vector
{
    void *data;
    void **dataP;
    size_t capacity;
    size_t size;
    size_t elementSize;
    size_t initialReservedSize;
    SDL_bool resizable;
} Vector;

typedef void **VectorP;

Vector *VectorCreate(size_t elementSize, size_t initialReservedSize);
void VectorDestroy(Vector *vector);

VectorP VectorSecureDataPointer(Vector *vector);

void VectorResize(Vector *vector, size_t newSize);

void VectorPushBack(Vector *vector, void *element);
void VectorPopBack(Vector *vector);

void VectorErase(Vector *vector, size_t index);
void VectorEraseRange(Vector *vector, size_t start, size_t end);
void VectorEraseElement(Vector *vector, void *element);
void VectorClear(Vector *vector);

size_t VectorFind(Vector *vector, void *element);

void VectorCopy(Vector *dst, Vector *src);

#endif