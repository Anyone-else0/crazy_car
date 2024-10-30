#include "car_util.h"
#include "sys_handle.h"
#include "car_usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define CAR_LOG_BUF_SIZE (256)

void carErrorCanNotHadle()
{
    Error_Handler();
    return;
}

void carLogPrint(CarLogLevel_t lvl, const char *pFunc, const char *pFmt, ...)
{
    char logMsg[CAR_LOG_BUF_SIZE];
    int useLen = 0;
    char *pLvl = NULL;

    if (lvl == CAR_LOG_LVL_INFO) {
        pLvl = "I";
    } else if (lvl == CAR_LOG_LVL_WARN) {
        pLvl = "W";
    } else {
        pLvl = "E";
    }

    va_list args;
    va_start(args, pFmt);

    useLen += snprintf(logMsg + useLen, sizeof(logMsg), "[%s][%s] ", pLvl, pFunc);
    if (useLen >= (int)sizeof(logMsg) - 1) {
        carErrorCanNotHadle();
    }
    useLen += vsnprintf(logMsg + useLen, sizeof(logMsg) - useLen, pFmt, args);
    if (useLen >= (int)sizeof(logMsg) - 1) {
        carErrorCanNotHadle();
    }
    va_end(args);
    useLen += snprintf(logMsg + useLen, sizeof(logMsg) - useLen, "\r\n");
    if (useLen >= (int)sizeof(logMsg) - 1) {
        carErrorCanNotHadle();
    }

    HAL_UART_Transmit(&gUsartHdl, (uint8_t *)logMsg, strlen(logMsg), 1000);
    while (__HAL_UART_GET_FLAG(&gUsartHdl, UART_FLAG_TC) != SET) {
        ;
    }

    return;
}

void carLogInit()
{
    carUsartInit(&gUsartHdl);
    CAR_LOG_INFO("Log init success.");
}

void carSysInit()
{
    if (HAL_Init() != HAL_OK) {
        carErrorCanNotHadle();
    }
    SystemClock_Config();
}

void carDelayMs(uint32_t timeMs)
{
    HAL_Delay(timeMs);
    return;
}