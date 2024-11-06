#include "car_engine.h"
#include "car_engine_hw.h"
#include "car_util.h"

// TODO: pid实现和左右电机分开驱动做差速算法

typedef struct CarEnginePrvt
{
    CarEngineHw_t *pHw;
} CarEnginePrvt_t;

static CarEnginePrvt_t sgPrvt;
static CarEngine_t sgCarEngine;

static void carEngineSpeedSet(CarEngine_t *pEngine, uint16_t speed)
{
    CarEngineHw_t *pHw = pEngine->pPrvt->pHw;
    pHw->pfDrivePulseSet(pHw, CAR_DRIVE_LOC_LEFT, speed);
    pHw->pfDrivePulseSet(pHw, CAR_DRIVE_LOC_RIGHT, speed);
    CAR_LOG_DEBUG("Engine speed: %u.", speed);
    return;
}

static void carEngineAngleSet(CarEngine_t *pEngine, int16_t angle)
{
    CarEngineHw_t *pHw = pEngine->pPrvt->pHw;
    pHw->pfOrientPulseSet(pHw, angle);
    CAR_LOG_DEBUG("Engine angle: %u", angle);
    return;
}

static void carEngineFormard(CarEngine_t *pEngine)
{
    CarEngineHw_t *pHw = pEngine->pPrvt->pHw;
    pHw->pfDriveForward(pHw);
    CAR_LOG_DEBUG("Engine forward.");
    return;
}

static void carEngineBack(CarEngine_t *pEngine)
{
    CarEngineHw_t *pHw = pEngine->pPrvt->pHw;
    pHw->pfDriveBack(pHw);
    CAR_LOG_DEBUG("Engine back.");
    return;
}

static int32_t carEngineInit(CarEngine_t *pEngine)
{
    int32_t rc = CAR_OK;
    CarEngineHw_t *pHw = carEngineHwGet();
    pEngine->pPrvt->pHw = pHw;
    rc = pHw->pfInit(pHw);
    if (rc != CAR_OK) {
        goto l_end;
    }
    pHw->pfPwmStart(pHw);

l_end:
    return rc;
}

static void carEngineIdle(CarEngine_t *pEngine)
{
    CarEngineHw_t *pHw = pEngine->pPrvt->pHw;
    pHw->pfDriveIdle(pHw);
    CAR_LOG_DEBUG("Engine Idle.");
}

static CarEngine_t sgCarEngine = {
    .pfInit = carEngineInit,
    .pfSpeedSet = carEngineSpeedSet,
    .pfAngleSet = carEngineAngleSet,
    .pfForward = carEngineFormard,
    .pfBack = carEngineBack,
    .pfIdel = carEngineIdle,
    .pPrvt = &sgPrvt,
};

CarEngine_t *carEngineGet(void)
{
    return &sgCarEngine;
}
