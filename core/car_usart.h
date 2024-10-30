#ifndef __CAR_USART_H__
#define __CAR_USART_H__

#include "stm32f1xx_hal.h"
#include "car_list.h"

#define CAR_USART_BUF_SIZE (512)
#define CAR_USART_BUF_CAP (2)

extern UART_HandleTypeDef gUsartHdl;

typedef struct CarUsartBufNode
{
    CarListHead_t node;
    uint8_t buf[CAR_USART_BUF_SIZE];
    uint16_t nr;
} CarUsartBufNode_t;

typedef struct CarUsart {
    UART_HandleTypeDef *pUartHdl;
    CarUsartBufNode_t *pRsvMem;
    CarListHead_t rxDoneQ;
    CarListHead_t rxIdleQ;
} CarUart_t;

void carUsartInit(UART_HandleTypeDef *pUsartHdl);

#endif
