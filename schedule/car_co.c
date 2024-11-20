#include "car_co.h"
#include "car_list.h"
#include "stm32f1xx_hal.h"
#include <string.h>
// TODO: 实现记录栈，可让度
#define CAR_CM3_REG_NR (20)
#define CAR_CO_TASK_CAP (100)

typedef struct CarCoTask
{
    CarListHead_t head;
    // uint32_t nextReg[CAR_CM3_REG_NR];
    // uint32_t curReg[CAR_CM3_REG_NR];
    uint32_t execTickMs;
    void (*pfExec)(void *pCtx);
    void *pCtx;
} CarCoTask_t;

typedef enum CarCoSchSta
{
    CAR_CO_SCH_INIT,
    CAR_CO_SCH_RUNNING,
    CAR_CO_SCH_STOP,
} CarCoSchSta_t;

typedef struct CarCoSchPrvt
{
    CarCoTask_t resTask[CAR_CO_TASK_CAP];
    CarListHead_t freeQ;
    CarListHead_t todoQ;
    CarListHead_t gearQ;

    CarCoSchSta_t sta;
} CarCoSchPrvt_t;

static 

static uint32_t carCoSchGetMsTick(void)
{
    return HAL_GetTick();
}

static void carCoSchInit(CarCoSch_t *pSch)
{
    (void)memset(pSch->pPrvt->resTask, 0, sizeof(pSch->pPrvt->resTask));
    carListHeadInit(&pSch->pPrvt->freeQ);
    carListHeadInit(&pSch->pPrvt->todoQ);
    carListHeadInit(&pSch->pPrvt->gearQ);

    for (int32_t idx = 0; idx < (int32_t)(sizeof(pSch->pPrvt->resTask) / sizeof(pSch->pPrvt->resTask[0])); idx++)
    {
        CarCoTask_t *pTask = &pSch->pPrvt->resTask[idx];
        carListHeadInit(&pTask->head);
        carListAddTail(&pSch->pPrvt->freeQ, &pTask->head);
    }

    return;
}

static CarCoTask_t *carCoSchTaskGet(CarCoSch_t *pSch)
{
    CarCoTask_t *pTask = NULL;
    if (carListEmpty(&pSch->pPrvt->freeQ))
    {
        goto l_end;
    }

    CarListHead_t *pHead = carListPickHead(&pSch->pPrvt->freeQ);
    pTask = CAR_LIST_ENTRY(pHead, CarCoTask_t, head);

l_end:
    return pTask;
}

static void carCoSchTaskPost(CarCoSch_t *pSch, void (*pfExec)(void *pCtx), void *pCtx)
{
    CarCoTask_t *pTask = carCoSchTaskGet(pSch);
    pTask->pfExec = pfExec;
    pTask->pCtx = pCtx;
    carListAddTail(&pSch->pPrvt->todoQ, &pTask->head);
    return;
}

static void carCoSchGearPost(CarCoSch_t *pSch, void (*pfExec)(void *pCtx), void *pCtx, uint32_t timeOutMs)
{
    CarCoTask_t *pTask = carCoSchTaskGet(pSch);
    pTask->execTickMs = carCoSchGetMsTick() + timeOutMs;
    pTask->pfExec = pfExec;
    pTask->pCtx = pCtx;
    CarListHead_t *pCurHead = pSch->pPrvt->gearQ.pNext;
    while (pCurHead != &pSch->pPrvt->gearQ) {
        CarCoTask_t *pCurTask = CAR_LIST_ENTRY(pCurHead, CarCoTask_t, head);
        if ((int32_t)(pCurTask->execTickMs - pTask->execTickMs) > 0) {
            break;
        }
        pCurHead = pCurHead->pNext;
    }
    carListAdd(pCurHead->pPrev, pCurHead, &pTask->head);
    return;
}

static void carCoSchRun(CarCoSch_t *pSch)
{
    pSch->pPrvt->sta = CAR_CO_SCH_RUNNING;

    while (pSch->pPrvt->sta == CAR_CO_SCH_RUNNING)
    {
        while (!carListEmpty(&pSch->pPrvt->todoQ))
        {
            CarListHead_t *pHead = carListPickHead(&pSch->pPrvt->todoQ);
            CarCoTask_t *pTask = CAR_LIST_ENTRY(pHead, CarCoTask_t, head);
            pTask->pfExec(pTask->pCtx);
            pTask->pfExec = NULL;
            pTask->pCtx = NULL;
            carListHeadInit(&pTask->head);
            carListAddTail(&pSch->pPrvt->freeQ, &pTask->head);
        }
        while (!carListEmpty(&pSch->pPrvt->gearQ))
        {
            CarListHead_t *pHead = carListHead(&pSch->pPrvt->gearQ);
            CarCoTask_t *pTask = CAR_LIST_ENTRY(pHead, CarCoTask_t, head);
            uint32_t curTick = carCoSchGetMsTick();
            if ((int32_t)(curTick - pTask->execTickMs) >= 0)
            {
                pHead = carListPickHead(&pSch->pPrvt->gearQ);
                pTask->pfExec(pTask->pCtx);
                pTask->pfExec = NULL;
                pTask->pCtx = NULL;
                carListHeadInit(&pTask->head);
                carListAddTail(&pSch->pPrvt->freeQ, &pTask->head);
            }
            else
            {
                break;
            }
        }
    }
    return;
}

static void carCoSchExit(CarCoSch_t *pSch)
{
    pSch->pPrvt->sta = CAR_CO_SCH_STOP;
    return;
}

static CarCoSchPrvt_t sgPrvt;

static CarCoSch_t sgCarCoSch = {
    .pPrvt = &sgPrvt,
    .pfInit = carCoSchInit,
    .pfCoSchRun = carCoSchRun,
    .pfCoSchExit = carCoSchExit,
    .pfCoPost = carCoSchTaskPost,
    .pfCoPostDelay = carCoSchGearPost,
};

CarCoSch_t *carCoSchGet(void)
{
    return &sgCarCoSch;
}

/*
static void carCoSwapStack(void)
{
    return;
}

void carCoYield(void)
{
    // TODO 待实现切换栈
    return;
}
*/