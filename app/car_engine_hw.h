#ifndef __CAR_ENGINE_HW_H__
#define __CAR_ENGINE_HW_H__

#include "stdint.h"

typedef enum CarEngineDriveLoc
{
    CAR_DRIVE_LOC_LEFT,
    CAR_DRIVE_LOC_RIGHT,
} CarEngineDriveLoc_t;

typedef struct CarEngineHw
{
    struct CarEngineHwPrvt *pPrvt;
    uint32_t orientZeroPulse;
    uint32_t orientFullPulse;
    uint32_t orientLeasePulse;
    uint32_t driveFullPulse;

    int32_t (*pfInit)(struct CarEngineHw *pHw);
    void (*pfPwmStart)(struct CarEngineHw *pHw);
    void (*pfOrientPulseSet)(struct CarEngineHw *pHw, uint32_t cnt);
    void (*pfDrivePulseSet)(struct CarEngineHw *pHw, CarEngineDriveLoc_t loc, uint32_t cnt);
    void (*pfDriveForward)(struct CarEngineHw *pHw);
    void (*pfDriveBack)(struct CarEngineHw *pHw);
    void (*pfDriveIdle)(struct CarEngineHw *pHw);
} CarEngineHw_t;

CarEngineHw_t *carEngineHwGet(void);

#endif