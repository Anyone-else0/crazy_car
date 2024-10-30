#include "stm32f1xx_hal.h"
#include "sys_handle.h"
#include "car_log.h"
#include "car_gpio.h"
#include "car_tim.h"
#include "car_usart.h"

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

int main(void)
{
    uint32_t mainPulse = 0;
    HAL_Init();
    SystemClock_Config();
    carLogInit();
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 10000 - 1);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 10000 - 1);

    while (1) {
        delayMs(500);
        CAR_LOG_INFO("Hello there, mainPulse: %lu.", mainPulse);
        mainPulse++;
        unsigned int delay = 500;
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
        //__HAL_TIM_SetCompare(&htim1 , TIM_CHANNEL_1 , 1000-1);
        delayMs(delay);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
        //__HAL_TIM_SetCompare(&htim1 , TIM_CHANNEL_1 , 1500-1);
        delayMs(delay);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
        //__HAL_TIM_SetCompare(&htim1 , TIM_CHANNEL_1 , 2000-1);
        delayMs(delay);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
        //__HAL_TIM_SetCompare(&htim1 , TIM_CHANNEL_1 , 1500-1);
        delayMs(delay);
    }
}
