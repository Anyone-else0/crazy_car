#ifndef __CAR_ENGINE_H__
#define __CAR_ENGINE_H__

#include <stdint.h>

typedef struct CarEngine
{
    struct CarEnginePrvt *pPrvt;
    uint16_t expSpeed;
    uint16_t curSpeed;
    int32_t (*pfInit)(struct CarEngine *pEngine);
    void (*pfSpeedSet)(struct CarEngine *pEngine, uint16_t speed);
    void (*pfAngleSet)(struct CarEngine *pEngine, int16_t angle);
    void (*pfForward)(struct CarEngine *pEngine);
    void (*pfBack)(struct CarEngine *pEngine);
    void (*pfIdel)(struct CarEngine *pEngine);
} CarEngine_t;

CarEngine_t *carEngineGet();

#endif