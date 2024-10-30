#include "car_usart.h"
#include "sys_handle.h"
#include <stdlib.h>

#define CAR_USART UART5
#define CAR_USART_IRQN UART5_IRQn
#define CAR_USART_IRQ_HANDLER UART5_IRQHandler
#define CAR_USART_RX_GPIO GPIOD
#define CAR_USART_RX_GPIO_PIN GPIO_PIN_2
#define CAR_USART_TX_GPIO GPIOC
#define CAR_USART_TX_GPIO_PIN GPIO_PIN_12

UART_HandleTypeDef gUsartHdl;

void CAR_USART_IRQ_HANDLER(void)
{
    HAL_UART_IRQHandler(&gUsartHdl);
}

void carUartMspInit(UART_HandleTypeDef *pUsartHdl)
{
    GPIO_InitTypeDef gpioInit = {0};
    __HAL_RCC_UART5_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    gpioInit.Pin = CAR_USART_TX_GPIO_PIN;
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CAR_USART_TX_GPIO, &gpioInit);

    gpioInit.Pin = CAR_USART_RX_GPIO_PIN;
    gpioInit.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(CAR_USART_RX_GPIO, &gpioInit);

    HAL_NVIC_SetPriority(CAR_USART_IRQN, 0, 0);
    HAL_NVIC_EnableIRQ(CAR_USART_IRQN);
}

void carUartMspDeInit(UART_HandleTypeDef *pUsartHdl)
{
    __HAL_RCC_UART5_CLK_DISABLE();
    HAL_GPIO_DeInit(CAR_USART_TX_GPIO, CAR_USART_TX_GPIO_PIN);
    HAL_GPIO_DeInit(CAR_USART_RX_GPIO, CAR_USART_RX_GPIO_PIN);
    HAL_NVIC_DisableIRQ(CAR_USART_IRQN);
}

void carUartCpltCb(UART_HandleTypeDef *pUsartHdl)
{
    return;
}

void carUartRxEventCb(UART_HandleTypeDef *pUsartHdl, uint16_t Size)
{
    return;
}

void carUsartInit(UART_HandleTypeDef *pUsartHdl)
{
    pUsartHdl->Instance = CAR_USART;
    pUsartHdl->Init.BaudRate = 115200;
    pUsartHdl->Init.WordLength = UART_WORDLENGTH_8B;
    pUsartHdl->Init.StopBits = UART_STOPBITS_1;
    pUsartHdl->Init.Parity = UART_PARITY_NONE;
    pUsartHdl->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    pUsartHdl->Init.Mode = UART_MODE_TX_RX;
    if (HAL_UART_RegisterCallback(pUsartHdl, HAL_UART_MSPINIT_CB_ID, carUartMspInit) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UART_RegisterCallback(pUsartHdl, HAL_UART_MSPDEINIT_CB_ID, carUartMspDeInit) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UART_Init(pUsartHdl) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UART_RegisterCallback(pUsartHdl, HAL_UART_RX_COMPLETE_CB_ID, carUartCpltCb) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UART_RegisterRxEventCallback(pUsartHdl, carUartRxEventCb) != HAL_OK) {
        Error_Handler();
    }
    // HAL_UARTEx_ReceiveToIdle_IT(pUsartHdl, (uint8_t *)g_rx_buffer, RXBUFFERSIZE);
}

CarUart_t *carUsartCreate(void)
{
    CarUart_t *pUsart = (CarUart_t *)malloc(sizeof(*pUsart));
    if (pUsart == NULL) {
        // error handle
    }
    pUsart->pUartHdl = &gUsartHdl;

    carListNodeInit(&pUsart->rxDoneQ);
    carListNodeInit(&pUsart->rxIdleQ);

    if (pUsart->pUartHdl->Instance != CAR_USART) {
        carUsartInit(pUsart->pUartHdl);
    }
    // HAL_UARTEx_ReceiveToIdle_IT(&pUsart->pUartHdl, (uint8_t *)g_rx_buffer, RXBUFFERSIZE);
    return pUsart;
}

void carUsartDestroy(CarUart_t *pUsart)
{
    return;
}
