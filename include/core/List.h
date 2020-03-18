#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#include <SDL2/SDL_mutex.h>

typedef struct Node
{
    void *data;
    size_t size;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct ListT
{
    Node *front;
    Node *back;
    size_t len;

    SDL_mutex *lock;
} List;

List ListCreate();
void ListDestroy(List *list);

Node *ListPushFront(List *list, const void *data, const size_t size);
Node *ListPushBack(List *list, const void *data, const size_t size);

Node *ListPopFront(List *list);
Node *ListPopBack(List *list);

Node *ListInsert(List *list, const void *data, size_t size, const size_t index);
Node *ListErase(List *list, const size_t index);
Node *ListEraseNode(List *list, const Node *node);

size_t ListSearch(List *list, const void *key, const size_t size);

#endif