#ifndef __CAR_TIM_H__
#define __CAR_TIM_H__

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void MX_TIM1_Init(void);
void MX_TIM2_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#endif
