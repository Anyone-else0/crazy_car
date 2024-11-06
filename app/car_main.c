#include "car_util.h"
#include "car_engine.h"

int main(void)
{
    uint32_t mainPulse = 0;
    carSysInit();
    carLogInit();

    CarEngine_t *pEngine = carEngineGet();
    int32_t rc = pEngine->pfInit(pEngine);
    if (rc != CAR_OK) {
        carErrorCanNotHadle();
    }
    pEngine->pfSpeedSet(pEngine, 10000 - 1);

    while (1) {
        CAR_LOG_INFO("Hello there, mainPulse: %lu.", mainPulse);
        mainPulse++;
        //carDelayMs(1000);
        //continue;
        uint32_t delay = 800;
        pEngine->pfForward(pEngine);
        pEngine->pfAngleSet(pEngine, 1000-1);
        carDelayMs(delay);
        pEngine->pfIdel(pEngine);
        pEngine->pfAngleSet(pEngine, 1500-1);
        carDelayMs(delay);
        pEngine->pfBack(pEngine);
        pEngine->pfAngleSet(pEngine, 2000-1);
        carDelayMs(delay);
        pEngine->pfIdel(pEngine);
        pEngine->pfAngleSet(pEngine, 1500-1);
        carDelayMs(delay);
    }
}
