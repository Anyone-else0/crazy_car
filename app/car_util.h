#ifndef __CAR_UTIL_H__
#define __CAR_UTIL_H__

#include <stdint.h>

#define CAR_DEBUG

typedef enum CarRet
{
    CAR_OK = 0,
    CAR_ERR,
    CAR_HAL_ERR,
} CarRet_t;

typedef enum CarLogLevel
{
    CAR_LOG_LVL_DEBUG,
    CAR_LOG_LVL_INFO,
    CAR_LOG_LVL_WARN,
    CAR_LOG_LVL_ERROR,
} CarLogLevel_t;

void carLogPrint(CarLogLevel_t lvl, const char *pFunc, const char *pFmt, ...);
void carLogInit();

#define CAR_LOG_INFO(fmt, ...) carLogPrint(CAR_LOG_LVL_INFO, __func__, fmt, ##__VA_ARGS__)
#define CAR_LOG_WARN(fmt, ...) carLogPrint(CAR_LOG_LVL_WARN, __func__, fmt, ##__VA_ARGS__)
#define CAR_LOG_ERROR(fmt, ...) carLogPrint(CAR_LOG_LVL_ERROR, __func__, fmt, ##__VA_ARGS__)

#ifdef CAR_DEBUG
#define CAR_LOG_DEBUG(fmt, ...) carLogPrint(CAR_LOG_LVL_DEBUG, __func__, fmt, ##__VA_ARGS__)
#else
#define CAR_LOG_DEBUG(fmt, ...)
#endif

#define CAR_UNREFERRENCED(x) (void)x

void carErrorCanNotHadle();
void carSysInit();
void carDelayMs(uint32_t timeMs);

#endif