#include "car_stub.h"
#include "car_co.h"
#include "ut_def.h"
#include <assert.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

static uint32_t sgCarCoTickStub = 4294967295-3000;
static bool sgCarTickStubSta = true;

uint32_t HAL_GetTick(void)
{
    return sgCarCoTickStub;
}

static void *carCoSchTickThStub(void *pArg)
{
    while (sgCarTickStubSta == true) {
        usleep(1000);
        sgCarCoTickStub++;
    }
    CAR_UNREFERRENCED(pArg);
    return NULL;
}

static void *carCoSchThStub(void *pArg)
{
    CarCoSch_t *pSch = carCoSchGet();
    pSch->pfCoSchRun(pSch);
    CAR_UNREFERRENCED(pArg);
    return NULL;
}

void carCoSchStubStop()
{
    CarCoSch_t *pSch = carCoSchGet();
    pSch->pfCoSchExit(pSch);
    sgCarTickStubSta = false;
}

void carCoSchStubStart()
{
    pthread_t tickThid;
    pthread_t schThid;
    sgCarTickStubSta = true;
    int rc = pthread_create(&tickThid, NULL, carCoSchTickThStub, NULL);
    assert(rc == 0);
    CarCoSch_t *pSch = carCoSchGet();
    pSch->pfInit(pSch);
    rc = pthread_create(&schThid, NULL, carCoSchThStub, NULL);
    assert(rc == 0);
}

