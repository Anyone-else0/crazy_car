#include "stm32f1xx_hal.h"
#include "sys_handle.h"
#include "tim.h"
#include "gpio.h"

void delayUs(unsigned int us)
{
  unsigned int count = us * 72 / 5;
  while (count--) {
    __NOP();
  }
}

void delayMs(unsigned int ms)
{
  while (ms--) {
    delayUs(1000);
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();

  HAL_TIM_PWM_Start (&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start (&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start (&htim2, TIM_CHANNEL_2);

  while (1)
  {
    unsigned int delay = 200;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    __HAL_TIM_SetCompare(&htim1 , TIM_CHANNEL_1 , 1000-1);
    delayMs(delay);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    __HAL_TIM_SetCompare(&htim1 , TIM_CHANNEL_1 , 1500-1);
    delayMs(delay);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    __HAL_TIM_SetCompare(&htim1 , TIM_CHANNEL_1 , 2000-1);
    delayMs(delay);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    __HAL_TIM_SetCompare(&htim1 , TIM_CHANNEL_1 , 1500-1);
    delayMs(delay);
  }
}
