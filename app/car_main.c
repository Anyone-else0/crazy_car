#include "car_util.h"
#include "car_engine.h"
#include "car_co.h"
#include <stddef.h>

static void carEngineTest(void *pCtx)
{
    CarEngine_t *pEngine = (CarEngine_t *)pCtx;
    static uint32_t cnt = 0;

    CAR_LOG_INFO("Hello there, Engine task run.");

    if (cnt == 0) {
        pEngine->pfForward(pEngine);
        pEngine->pfAngleSet(pEngine, 1000 - 1);
    }
    if (cnt == 1) {
        pEngine->pfIdel(pEngine);
        pEngine->pfAngleSet(pEngine, 1500 - 1);
    }
    if (cnt == 2) {
        pEngine->pfBack(pEngine);
        pEngine->pfAngleSet(pEngine, 2000 - 1);
    }
    if (cnt == 3) {
        pEngine->pfIdel(pEngine);
        pEngine->pfAngleSet(pEngine, 1500 - 1);
    }

    cnt++;
    if (cnt > 3) {
        cnt = 0;
    }

    CarCoSch_t *pSch = carCoSchGet();
    pSch->pfCoPostDelay(pSch, carEngineTest, pEngine, 1000);
    return;
}

static void carHeartBeat(void *pCtx)
{
    static uint32_t beat = 0;
    CAR_LOG_INFO("Car heart beat: %lu", beat);
    beat++;
    CarCoSch_t *pSch = carCoSchGet();
    pSch->pfCoPostDelay(pSch, carHeartBeat, NULL, 1000);
    CAR_UNREFERRENCED(pCtx);
    return;
}

int main(void)
{
    carSysInit();
    carLogInit();

    CarEngine_t *pEngine = carEngineGet();
    int32_t rc = pEngine->pfInit(pEngine);
    if (rc != CAR_OK) {
        carErrorCanNotHadle();
    }
    pEngine->pfSpeedSet(pEngine, 10000 - 1);

    CarCoSch_t *pSch = carCoSchGet();
    pSch->pfInit(pSch);
    pSch->pfCoPostDelay(pSch, carEngineTest, pEngine, 1000);
    pSch->pfCoPostDelay(pSch, carHeartBeat, NULL, 500);
    pSch->pfCoSchRun(pSch);
    return 0;
}
