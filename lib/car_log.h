#ifndef __CAR_LOG_H__
#define __CAR_LOG_H__

#include "car_usart.h"
#include <stdio.h>
#include <string.h>

#define CAR_LOG_BUF_SIZE (128)

extern char gLogBuf[CAR_LOG_BUF_SIZE];

#define CAR_LOG_INFO(format, ...)                                                                 \
    do {                                                                                          \
        snprintf(gLogBuf, CAR_LOG_BUF_SIZE, "[INFO][%s]" format "\r\n", __func__, ##__VA_ARGS__); \
        HAL_UART_Transmit(&gUsartHdl, (uint8_t *)gLogBuf, strlen(gLogBuf), 1000);                 \
        while (__HAL_UART_GET_FLAG(&gUsartHdl, UART_FLAG_TC) != SET) {                            \
            ;                                                                                     \
        }                                                                                         \
    } while (0);

#define CAR_LOG_ERROR(format, ...)                                                                 \
    do {                                                                                           \
        snprintf(gLogBuf, CAR_LOG_BUF_SIZE, "[ERROR][%s]" format "\r\n", __func__, ##__VA_ARGS__); \
        HAL_UART_Transmit(&gUsartHdl, (uint8_t *)gLogBuf, strlen(gLogBuf), 1000);                  \
        while (__HAL_UART_GET_FLAG(&gUsartHdl, UART_FLAG_TC) != SET) {                             \
            ;                                                                                      \
        }                                                                                          \
    } while (0);

static inline void carLogInit()
{
    carUsartInit(&gUsartHdl);
    CAR_LOG_INFO("Log init success.");
}
#endif