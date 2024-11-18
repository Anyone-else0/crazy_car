#include "car_co_ut.h"
#include "car_co.h"
#include "car_stub.h"
#include "stm32f1xx_hal.h"
#include <unistd.h>

void coTest(void *pCtx)
{
    int32_t *pCnt = (int32_t *)pCtx;
    (*pCnt)++;
}

static void carCoTest(void)
{
    carCoSchStubStart();
    int32_t coTestCnt = 0;
    CarCoSch_t *pSch = carCoSchGet();
    {
        for (int32_t idx = 0; idx < 5; idx++) {
            pSch->pfCoPost(pSch, coTest, &coTestCnt);
        }
        usleep(1000);
        UT_ASSERT(coTestCnt == 5);
    }
    {
        for (int32_t idx = 0; idx < 5; idx++) {
            pSch->pfCoPostDelay(pSch, coTest, &coTestCnt, 1000 * (idx + 1));
        }
        for (int32_t idx = 0; idx < 5; idx++) {
            uint32_t curTick = HAL_GetTick();
            while (((int32_t)(HAL_GetTick() - curTick)) < 1002) {
                ;
            }
            printf("coTestCnt: %d.\n", coTestCnt);
            printf("Tick: %u.\n", HAL_GetTick());
            UT_ASSERT(coTestCnt == 6 + idx);
        }
    }
    carCoSchStubStop();
}

UtCase_t gCarCoSuit[] = {
    {"carCoTest", carCoTest},
    {NULL, NULL}
};