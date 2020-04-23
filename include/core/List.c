#include "List.h"

void NodeDelete(Node *node)
{
    SDL_free(node->data);
    SDL_free(node);
}

List ListCreate()
{
    List ret;
    ret.front = NULL;
    ret.back = NULL;
    ret.len = 0;
    ret.lock = SDL_CreateMutex();
    return ret;
}
void ListDestroy(List *list)
{
    SDL_DestroyMutex(list->lock);
    ListClear(list);
}

Node *ListPushFront(List *list, const void *data, const size_t size)
{
    SDL_LockMutex(list->lock);
    Node *add = MALLOC(Node);
    add->prev = NULL;
    add->data = SDL_malloc(size);
    add->size = size;
    SDL_memcpy(add->data, data, size);

    if (list->front)
    {
        list->front->prev = add;
        add->next = list->front;
        list->front = add;
    }
    else
    {
        add->next = NULL;
        list->front = add;
        list->back = add;
    }
    list->len++;
    SDL_UnlockMutex(list->lock);
    return list->front;
}
Node *ListPushBack(List *list, const void *data, size_t size)
{
    Node *add = MALLOC(Node);
    add->next = NULL;
    add->data = SDL_malloc(size);
    add->size = size;
    SDL_memcpy(add->data, data, size);

    if (list->back)
    {
        list->back->next = add;
        add->prev = list->back;
        list->back = add;
    }
    else
    {
        add->prev = NULL;
        list->front = add;
        list->back = add;
    }
    list->len++;
    SDL_UnlockMutex(list->lock);
    return list->back;
}

Node *ListPopFront(List *list)
{
    SDL_LockMutex(list->lock);
    if (list->len > 0)
    {
        Node *pop = list->front;
        if (pop->next)
        {
            pop->next->prev = NULL;
            list->front = pop->next;
        }
        else
        {
            list->front = NULL;
            list->back = NULL;
        }
        NodeDelete(pop);
        list->len--;
        if (list->len)
        {
            SDL_UnlockMutex(list->lock);
            return list->front;
        }
        else
        {
            SDL_UnlockMutex(list->lock);
            return NULL;
        }
    }
    else
    {
        SDL_UnlockMutex(list->lock);
        return NULL;
    }
}
Node *ListPopBack(List *list)
{
    SDL_LockMutex(list->lock);
    if (list->len > 0)
    {
        Node *pop = list->back;
        if (pop->prev)
        {
            pop->prev->next = NULL;
            list->back = pop->prev;
        }
        else
        {
            list->front = NULL;
            list->back = NULL;
        }
        NodeDelete(pop);
        list->len--;
        if (list->len)
        {
            SDL_UnlockMutex(list->lock);
            return list->back;
        }
        else
        {
            SDL_UnlockMutex(list->lock);
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

Node *ListInsert(List *list, const void *data, size_t size, const size_t index)
{
    SDL_LockMutex(list->lock);
    if (index < list->len && list->len > 0)
    {
        if (index == list->len - 1)
        {
            SDL_UnlockMutex(list->lock);
            return ListPushBack(list, data, size);
        }
        else if (index == 0)
        {
            SDL_UnlockMutex(list->lock);
            return ListPushFront(list, data, size);
        }
        else
        {
            Node *hit = list->front;
            for (size_t i = 0; i != index; hit = hit->next, i++)
                ;
            Node *add = MALLOC(Node);
            add->next = hit;
            add->prev = hit->prev;
            add->data = SDL_malloc(size);
            add->size = size;
            SDL_memcpy(add->data, data, size);

            hit->prev->next = hit;
            hit->prev = hit;

            SDL_UnlockMutex(list->lock);
            return hit;
        }
    }
    else
    {
        SDL_UnlockMutex(list->lock);
        return NULL;
    }
}
Node *ListErase(List *list, const size_t index)
{
    SDL_LockMutex(list->lock);
    if (index < list->len && list->len > 0)
    {
        if (index == list->len - 1)
        {
            SDL_UnlockMutex(list->lock);
            return ListPopBack(list);
        }
        else if (index == 0)
        {
            SDL_UnlockMutex(list->lock);
            return ListPopFront(list);
        }
        else
        {
            Node *hit = list->front;
            for (size_t i = 0; i != index; hit = hit->next, i++)
                ;
            Node *ret = hit->next;
            hit->prev->next = hit->next;
            hit->next->prev = hit->prev;
            NodeDelete(hit);

            SDL_UnlockMutex(list->lock);
            return ret;
        }
    }
    else
    {
        SDL_UnlockMutex(list->lock);
        return NULL;
    }
}

Node *ListEraseNode(List *list, const Node *node)
{
    SDL_LockMutex(list->lock);
    size_t index = 0;
    if (list->len > 0 && node)
        for (Node *node_local = list->front; node_local; node_local = node_local->next, index++)
            if (node == node_local)
            {
                SDL_UnlockMutex(list->lock);
                return ListErase(list, index);
            }

    SDL_UnlockMutex(list->lock);
    return NULL;
}

size_t ListSearch(List *list, const void *key, const size_t size)
{
    SDL_LockMutex(list->lock);
    if (list->len > 0)
    {
        size_t index = 0;
        char *cmp1 = MALLOC_N(char, size + 1);
        SDL_memcpy(cmp1, key, size);
        cmp1[size] = 0;

        for (Node *node = list->front; node; node = node->next, index++)
        {
            char *cmp2 = MALLOC_N(char, node->size + 1);
            SDL_memcpy(cmp2, node->data, size);
            cmp2[size] = 0;

            if (!strcmp(cmp1, cmp2))
            {
                SDL_free(cmp1);
                SDL_free(cmp2);
                SDL_UnlockMutex(list->lock);
                return index;
            }
            SDL_free(cmp2);
        }
        SDL_free(cmp1);
    }
    SDL_UnlockMutex(list->lock);
    return list->len;
}

Node *ListGet(List *list, const size_t index)
{
    size_t i = 0;
    for (Node *node = list->front; node; node = node->next, i++)
    {
        if (i == index)
            return node;
    }
    return NULL;
}

void ListClear(List *list)
{
    if (list->len > 0)
        for (Node *node = list->front; node; node = node->next)
            NodeDelete(node);
    list->front = NULL;
    list->back = NULL;
    list->len = 0;
}