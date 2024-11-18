#ifndef __CAR_LIST_H__
#define __CAR_LIST_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct CarListHead {
    struct CarListHead *pNext;
    struct CarListHead *pPrev;
} CarListHead_t;

static inline void carListHeadInit(CarListHead_t *pHead)
{
    pHead->pNext = pHead;
    pHead->pPrev = pHead;
}

static inline bool carListEmpty(CarListHead_t *pList)
{
    return pList->pNext == pList && pList->pPrev == pList;
}

static inline void carListDel(CarListHead_t *pHead)
{
    pHead->pPrev->pNext = pHead->pNext;
    pHead->pNext->pPrev = pHead->pPrev;
    return;
}

static inline void carListAdd(CarListHead_t *pPrev, CarListHead_t *pNext, CarListHead_t *pNew)
{
    pNew->pNext = pNext;
    pNew->pPrev = pPrev;
    pPrev->pNext = pNew;
    pNext->pPrev = pNew;
    return;
}

static inline void carListAddTail(CarListHead_t *pList, CarListHead_t *pNew)
{
    carListAdd(pList->pPrev, pList, pNew);
    return;
}

static inline void carListAddHead(CarListHead_t *pList, CarListHead_t *pNew)
{
    carListAdd(pList, pList->pNext, pNew);
    return;
}

static inline CarListHead_t *carListHead(CarListHead_t *pList)
{
    if (carListEmpty(pList)) {
        return NULL;
    }
    return pList->pNext;
}

static inline CarListHead_t *carListPickHead(CarListHead_t *pList)
{
    CarListHead_t *pHead = carListHead(pList);
    if (pHead == NULL) {
        return NULL;
    }
    carListDel(pHead);
    return pHead;
}

static inline CarListHead_t *carListTail(CarListHead_t *pList)
{
    if (carListEmpty(pList)) {
        return NULL;
    }
    return pList->pPrev;
}

static inline CarListHead_t *carListPickTail(CarListHead_t *pList)
{
    CarListHead_t *pHead = carListTail(pList);
    if (pHead == NULL) {
        return NULL;
    }
    carListDel(pHead);
    return pHead;
}

#define CAR_LIST_ENTRY(ptr, type, member) ((type *)(((void *)ptr) - offsetof(type, member)))

#define CAR_LIST_FOREACH(pPos, pHead) for (CarListHead_t *pPos = (pHead)->pNext; pPos != (pHead); pPos = pPos->pNext)

#endif