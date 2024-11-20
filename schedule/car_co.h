#ifndef __CAR_CO_H__
#define __CAR_CO_H__

#include <stdint.h>

typedef struct CarCoSch
{
    struct CarCoSchPrvt *pPrvt;
    void (*pfInit)(struct CarCoSch *pSch);
    void (*pfCoSchRun)(struct CarCoSch *pSch);
    void (*pfCoSchExit)(struct CarCoSch *pSch);
    void (*pfCoPost)(struct CarCoSch *pSch, void (*pfExec)(void *pCtx), void *pCtx);
    void (*pfCoPostDelay)(struct CarCoSch *pSch, void (*pfExec)(void *pCtx), void *pCtx, uint32_t delayMs);
} CarCoSch_t;

CarCoSch_t *carCoSchGet(void);

#endif