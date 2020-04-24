#ifndef LIST_H
#define LIST_H

#include "Dependencies.h"

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

/// Insert data in the front of the list
/// \param data: pointer to the data to be inserted
/// \param size: size of pointed out data
Node *ListPushFront(List *list, const void *data, const size_t size);
/// Insert data in the back of the list
/// \param data: pointer to the data to be inserted
/// \param size: size of pointed out data
Node *ListPushBack(List *list, const void *data, const size_t size);

/// Removes the rear data from the list
Node *ListPopFront(List *list);
/// Removes the front data from the list
Node *ListPopBack(List *list);

/// Inserts data in the list with specified index
/// \param data: pointer to the data to be inserted
/// \param size: size of pointed out data
/// \param index: the resulting index of the inserted data
Node *ListInsert(List *list, const void *data, size_t size, const size_t index);
/// Removes data in the list with specified index
/// \param index: index of data to be removed
Node *ListErase(List *list, const size_t index);
Node *ListEraseNode(List *list, const Node *node);

/// Searches the list with a key, returning the index
/// \param key: to be compared to data (should match data type)
/// \param size: size of key/data
size_t ListSearch(List *list, const void *key, const size_t size);

Node *ListGet(List *list, const size_t index);

/// Clear the contents of the list
void ListClear(List *list);

#endif