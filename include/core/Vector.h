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
} Vector;

typedef void **VectorP;

Vector *VectorCreate(size_t elementSize, size_t initialReservedSize);
void VectorDestroy(Vector *vector);

VectorP VectorSecureDataPointer(Vector *vector);

void VectorPushBack(Vector *vector, void *element);
void VectorPopBack(Vector *vector);

void VectorErase(Vector *vector, size_t index);

#endif