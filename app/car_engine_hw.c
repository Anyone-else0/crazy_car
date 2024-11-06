#include "car_engine_hw.h"
#include "car_util.h"
#include "stm32f1xx_hal.h"

#define CAR_ENGINE_ORIENT_TMR TIM1
#define CAR_ENGINE_ORIENT_TMR_GPIO GPIOE
#define CAR_ENGINE_ORIENT_TMR_GPIO_PIN GPIO_PIN_9

#define CAR_ENGINE_DRIVE_TMR TIM2
#define CAR_ENGINE_DRIVE_TMR_GPIO GPIOA
#define CAR_ENGINE_DRIVE_TMR_GPIO_PIN_CH1 GPIO_PIN_0
#define CAR_ENGINE_DRIVE_TMR_GPIO_PIN_CH2 GPIO_PIN_1

static TIM_HandleTypeDef sgCarEngineOrientTmr;
static TIM_HandleTypeDef sgCarEngineDriveTmr;

typedef struct CarDriveIO
{
    GPIO_TypeDef *pGpioHdl;
    uint16_t in1Pin;
    uint16_t in2Pin;
} CarDriveIO_t;

typedef struct CarEngineHwPrvt {
    TIM_HandleTypeDef *pOrientTmr;
    TIM_HandleTypeDef *pDriveTmr;
    CarDriveIO_t driveL;
    CarDriveIO_t driveR;
} CarEngineHwPrvt_t;

static CarEngineHwPrvt_t sgPrvt = {
    .pOrientTmr = &sgCarEngineOrientTmr,
    .pDriveTmr = &sgCarEngineDriveTmr,
    .driveL.pGpioHdl = GPIOA,
    .driveL.in1Pin = GPIO_PIN_4,
    .driveL.in2Pin = GPIO_PIN_5,
    .driveR.pGpioHdl = GPIOC,
    .driveR.in1Pin = GPIO_PIN_4,
    .driveR.in2Pin = GPIO_PIN_5,
};

static void carEngineOrientTmrMspInit(TIM_HandleTypeDef *pTmr)
{
    __HAL_RCC_TIM1_CLK_ENABLE();
    CAR_LOG_INFO("Engine orient timmer msp init ok.");
    CAR_UNREFERRENCED(pTmr);
}

static void carEngineOrientTmrMspDeInit(TIM_HandleTypeDef *pTmr)
{
    __HAL_RCC_TIM1_CLK_DISABLE();
    CAR_LOG_INFO("Engine orient timmer deinit ok.");
    CAR_UNREFERRENCED(pTmr);
}

static void carEngineOrientTmrMspPostInit(TIM_HandleTypeDef *pTmr)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStruct.Pin = CAR_ENGINE_ORIENT_TMR_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CAR_ENGINE_ORIENT_TMR_GPIO, &GPIO_InitStruct);
    __HAL_AFIO_REMAP_TIM1_ENABLE();
    CAR_LOG_INFO("Engine orient timmer msp post init ok.");
    CAR_UNREFERRENCED(pTmr);
}

static int32_t carEngineOrientTmrInit(TIM_HandleTypeDef *pTmr)
{
    int32_t rc = CAR_OK;
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    pTmr->Instance = CAR_ENGINE_ORIENT_TMR;
    pTmr->Init.Prescaler = 72 - 1;
    pTmr->Init.CounterMode = TIM_COUNTERMODE_UP;
    pTmr->Init.Period = 20000;
    pTmr->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    pTmr->Init.RepetitionCounter = 0;
    pTmr->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_RegisterCallback(pTmr, HAL_TIM_BASE_MSPINIT_CB_ID, carEngineOrientTmrMspInit) != HAL_OK) {
        CAR_LOG_ERROR("Register func carEngineOrientTmrMspInit failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    if (HAL_TIM_RegisterCallback(pTmr, HAL_TIM_BASE_MSPDEINIT_CB_ID, carEngineOrientTmrMspDeInit) != HAL_OK) {
        CAR_LOG_ERROR("Register func carEngineOrientTmrMspDeInit failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    if (HAL_TIM_Base_Init(pTmr) != HAL_OK) {
        CAR_LOG_ERROR("Engine orient base init failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(pTmr, &sClockSourceConfig) != HAL_OK) {
        CAR_LOG_ERROR("Engine orient clock config failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    if (HAL_TIM_PWM_Init(pTmr) != HAL_OK) {
        CAR_LOG_ERROR("Engine orient pwm init failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(pTmr, &sMasterConfig) != HAL_OK) {
        CAR_LOG_ERROR("Engine orient master mode config failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1500;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(pTmr, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        CAR_LOG_ERROR("Engine orient pwm channel config failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(pTmr, &sBreakDeadTimeConfig) != HAL_OK) {
        CAR_LOG_ERROR("Engine orient dead time config failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    carEngineOrientTmrMspPostInit(pTmr);

    CAR_LOG_INFO("Engine orient timmer init ok.");

l_end:
    return rc;
}

static void carEngineDriveTmrMspInit(TIM_HandleTypeDef *pTmr)
{
    __HAL_RCC_TIM2_CLK_ENABLE();
    CAR_LOG_INFO("Engine drive timmer msp init ok.");
    CAR_UNREFERRENCED(pTmr);
}

static void carEngineDriveTmrMspDeInit(TIM_HandleTypeDef *pTmr)
{
    __HAL_RCC_TIM2_CLK_DISABLE();
    CAR_LOG_INFO("Engine drive timmer msp deinit ok.");
    CAR_UNREFERRENCED(pTmr);
}

static void carEngineDriveTmrMspPostInit(TIM_HandleTypeDef *pTmr)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = CAR_ENGINE_DRIVE_TMR_GPIO_PIN_CH1 | CAR_ENGINE_DRIVE_TMR_GPIO_PIN_CH2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CAR_ENGINE_DRIVE_TMR_GPIO, &GPIO_InitStruct);
    CAR_LOG_INFO("Engine drive timmer msp post init ok.");
    CAR_UNREFERRENCED(pTmr);
}

static int32_t carEngineDriveTmrInit(TIM_HandleTypeDef *pTmr)
{
    int32_t rc = CAR_OK;
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    pTmr->Instance = CAR_ENGINE_DRIVE_TMR;
    pTmr->Init.Prescaler = 72 - 1;
    pTmr->Init.CounterMode = TIM_COUNTERMODE_UP;
    pTmr->Init.Period = 10000 - 1;
    pTmr->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    pTmr->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_RegisterCallback(pTmr, HAL_TIM_BASE_MSPINIT_CB_ID, carEngineDriveTmrMspInit) != HAL_OK) {
        CAR_LOG_INFO("Register func carEngineDriveTmrMspInit failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    if (HAL_TIM_RegisterCallback(pTmr, HAL_TIM_BASE_MSPDEINIT_CB_ID, carEngineDriveTmrMspDeInit) != HAL_OK) {
        CAR_LOG_INFO("Register func carEngineDriveTmrMspDeInit failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }

    if (HAL_TIM_Base_Init(pTmr) != HAL_OK) {
        CAR_LOG_INFO("Engine drive base init failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(pTmr, &sClockSourceConfig) != HAL_OK) {
        CAR_LOG_INFO("Engine drive clock config failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    if (HAL_TIM_PWM_Init(pTmr) != HAL_OK) {
        CAR_LOG_INFO("Engine drive pwm init failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(pTmr, &sMasterConfig) != HAL_OK) {
        CAR_LOG_INFO("Engine drive master mode config failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 5000 - 1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(pTmr, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        CAR_LOG_INFO("Engine drive channel 1 config failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    if (HAL_TIM_PWM_ConfigChannel(pTmr, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
        CAR_LOG_INFO("Engine drive channel 2 config failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }

    carEngineDriveTmrMspPostInit(pTmr);

    CAR_LOG_INFO("Engine drive init ok");

l_end:
    return rc;
}

static void carEngineDriveIOInit(CarDriveIO_t *pLeftIO, CarDriveIO_t *pRightIO)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    HAL_GPIO_WritePin(pLeftIO->pGpioHdl, pLeftIO->in1Pin | pLeftIO->in2Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pRightIO->pGpioHdl, pRightIO->in1Pin | pRightIO->in2Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = pLeftIO->in1Pin | pLeftIO->in2Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(pLeftIO->pGpioHdl, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = pRightIO->in1Pin | pRightIO->in2Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(pRightIO->pGpioHdl, &GPIO_InitStruct);
    CAR_LOG_INFO("Engine drive io init ok");
}

static int32_t carEngineHwInit(CarEngineHw_t *pHw)
{
    int32_t rc = CAR_OK;
    carEngineDriveIOInit(&pHw->pPrvt->driveL, &pHw->pPrvt->driveR);
    if (carEngineOrientTmrInit(pHw->pPrvt->pOrientTmr) != CAR_OK) {
        goto l_end;
    }
    if (carEngineDriveTmrInit(pHw->pPrvt->pDriveTmr) != CAR_OK) {
        goto l_end;
    }

l_end:
    return rc;
}

static void carEngineHwPwmStart(CarEngineHw_t *pHw)
{
    if (HAL_TIM_PWM_Start(pHw->pPrvt->pOrientTmr, TIM_CHANNEL_1) != HAL_OK) {
        CAR_LOG_WARN("Engine orint pwm start failed.");
    }
    if (HAL_TIM_PWM_Start(pHw->pPrvt->pDriveTmr, TIM_CHANNEL_1) != HAL_OK) {
        CAR_LOG_WARN("Engine orint pwm start failed.");
    }
    if (HAL_TIM_PWM_Start(pHw->pPrvt->pDriveTmr, TIM_CHANNEL_2) != HAL_OK) {
        CAR_LOG_WARN("Engine orint pwm start failed.");
    }
}

static void carEngineHwOrentPulseSet(CarEngineHw_t *pHw, uint32_t cnt)
{
    __HAL_TIM_SetCompare(pHw->pPrvt->pOrientTmr, TIM_CHANNEL_1 , cnt);
}

static void carEngineHwDrivePulseSet(CarEngineHw_t *pHw, CarEngineDriveLoc_t loc, uint32_t cnt)
{
    if (loc == CAR_DRIVE_LOC_LEFT) {
        __HAL_TIM_SetCompare(pHw->pPrvt->pDriveTmr, TIM_CHANNEL_1, cnt);
    } else {
        __HAL_TIM_SetCompare(pHw->pPrvt->pDriveTmr, TIM_CHANNEL_2, cnt);
    }
    return;
}

static void carEngineHwDriveForward(CarEngineHw_t *pHw)
{
    HAL_GPIO_WritePin(pHw->pPrvt->driveL.pGpioHdl, pHw->pPrvt->driveL.in1Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pHw->pPrvt->driveL.pGpioHdl, pHw->pPrvt->driveL.in2Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pHw->pPrvt->driveR.pGpioHdl, pHw->pPrvt->driveR.in1Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pHw->pPrvt->driveR.pGpioHdl, pHw->pPrvt->driveR.in2Pin, GPIO_PIN_RESET);
}

static void carEngineHwDriveBack(CarEngineHw_t *pHw)
{
    HAL_GPIO_WritePin(pHw->pPrvt->driveL.pGpioHdl, pHw->pPrvt->driveL.in1Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pHw->pPrvt->driveL.pGpioHdl, pHw->pPrvt->driveL.in2Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pHw->pPrvt->driveR.pGpioHdl, pHw->pPrvt->driveR.in1Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pHw->pPrvt->driveR.pGpioHdl, pHw->pPrvt->driveR.in2Pin, GPIO_PIN_SET);
}

static void carEngineHwDriveIdle(CarEngineHw_t *pHw)
{
    HAL_GPIO_WritePin(pHw->pPrvt->driveL.pGpioHdl, pHw->pPrvt->driveL.in1Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pHw->pPrvt->driveL.pGpioHdl, pHw->pPrvt->driveL.in2Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pHw->pPrvt->driveR.pGpioHdl, pHw->pPrvt->driveR.in1Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pHw->pPrvt->driveR.pGpioHdl, pHw->pPrvt->driveR.in2Pin, GPIO_PIN_RESET);
}

static CarEngineHw_t sgCarEngineHw = {
    .pfInit = carEngineHwInit,
    .pfPwmStart = carEngineHwPwmStart,
    .pfOrientPulseSet = carEngineHwOrentPulseSet,
    .pfDrivePulseSet = carEngineHwDrivePulseSet,
    .pfDriveForward = carEngineHwDriveForward,
    .pfDriveBack = carEngineHwDriveBack,
    .pfDriveIdle = carEngineHwDriveIdle,
    .pPrvt = &sgPrvt,
};

CarEngineHw_t *carEngineHwGet(void)
{
    return &sgCarEngineHw;
}
