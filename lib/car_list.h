#ifndef __CAR_LIST_H__
#define __CAR_LIST_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct CarListHead {
    struct CarListHead *pNext;
    struct CarListHead *pPrev;
} CarListHead_t;

static inline void carListNodeInit(CarListHead_t *pNode)
{
    pNode->pNext = pNode;
    pNode->pPrev = pNode;
}

static inline bool carListEmpty(CarListHead_t *pHead)
{
    return pHead->pNext == pHead && pHead->pPrev == pHead;
}

static inline void carListDel(CarListHead_t *pNode)
{
    pNode->pPrev->pNext = pNode->pNext;
    pNode->pNext->pPrev = pNode->pPrev;
    return;
}

static inline void carListAdd(CarListHead_t *pNode, CarListHead_t *pPrev, CarListHead_t *pNext)
{
    pNode->pNext = pNext;
    pNode->pPrev = pPrev;
    pPrev->pNext = pNode;
    pNext->pPrev = pNode;
    return;
}

static inline void carListAddTail(CarListHead_t *pHead, CarListHead_t *pNode)
{
    carListAdd(pNode, pHead, pHead->pNext);
    return;
}

static inline void carListAddHead(CarListHead_t *pHead, CarListHead_t *pNode)
{
    carListAdd(pNode, pHead->pPrev, pHead);
    return;
}

static inline CarListHead_t *carListHead(CarListHead_t *pHead)
{
    if (carListEmpty(pHead)) {
        return NULL;
    }
    return pHead->pPrev;
}

static inline CarListHead_t *carListPickHead(CarListHead_t *pHead)
{
    CarListHead_t *pNode = carListHead(pHead);
    if (pNode == NULL) {
        return NULL;
    }
    carListDel(pNode);
    return pNode;
}

static inline CarListHead_t *carListTail(CarListHead_t *pHead)
{
    if (carListEmpty(pHead)) {
        return NULL;
    }
    return pHead->pNext;
}

static inline CarListHead_t *carListPickTail(CarListHead_t *pHead)
{
    CarListHead_t *pNode = carListTail(pHead);
    if (pNode == NULL) {
        return NULL;
    }
    carListDel(pNode);
    return pNode;
}

#define CAR_LIST_ENTRY(ptr, type, member) ((type *)(((void *)ptr) - offsetof(type, member)))

#define CAR_LIST_FOREACH(pPos, pHead) for (PeaListNode_t pPos = pHead->pNext; pPos != pHead; pPos = pPos->pNext)

#endif