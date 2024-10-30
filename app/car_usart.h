#ifndef __CAR_USART_H__
#define __CAR_USART_H__

#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef gUsartHdl;
void carUsartInit(UART_HandleTypeDef *pUsartHdl);

#endif
