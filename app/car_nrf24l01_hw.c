#include "car_nrf24l01_hw.h"
#include "car_util.h"

typedef struct CarNrfPrvt {
    SPI_HandleTypeDef *pSpiHdl;
    SPI_TypeDef spi;
    GPIO_TypeDef sckGpio;
    uint16_t sckPin;
    GPIO_TypeDef misoGpio;
    uint16_t misoPin;
    GPIO_TypeDef mosiGpio;
    uint16_t mosiPin;

    GPIO_TypeDef ceGpio;
    uint16_t cePin;
    GPIO_TypeDef csnGpio;
    uint16_t csnPin;
    GPIO_TypeDef irqGpio;
    uint16_t riqPin;
} CarNrfPrvt_t;

static SPI_HandleTypeDef sgCarNrfSpi;

static CarNrfPrvt_t sgPrvt = {
    .pSpiHdl = &sgCarNrfSpi,
    .spi = SPI1,
    .sckGpio = GPIOA,
    .sckPin = GPIO_PIN_5,
    .misoGpio = GPIOA,
    .misoPin = GPIO_PIN_6,
    .mosiGpio = GPIOA,
    .mosiPin = GPIO_PIN_7,
};
/*
PA5 --> SPI1_SCK
PA6 --> SPI1_MISO
PA7 --> SPI1_MOSI
*/

static void carNrfSpiMspInit(SPI_HandleTypeDef *pSpi)
{
    GPIO_InitTypeDef gpioPara = {0};

    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    gpioPara.Pin = sgPrvt.sckPin;
    gpioPara.Mode = GPIO_MODE_AF_PP;
    gpioPara.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(sgPrvt.sckGpio, &gpioPara);

    gpioPara.Pin = sgPrvt.misoPin;
    gpioPara.Mode = GPIO_MODE_INPUT;
    gpioPara.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(sgPrvt.misoGpio, &gpioPara);

    gpioPara.Pin = sgPrvt.mosiPin;
    gpioPara.Mode = GPIO_MODE_AF_PP;
    gpioPara.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(sgPrvt.mosiGpio, &gpioPara);

    CAR_UNREFERRENCED(pSpi);
}

static void carNrfSpiDeInit(SPI_HandleTypeDef *pSpi)
{
    __HAL_RCC_SPI1_CLK_DISABLE();
    HAL_GPIO_DeInit(sgPrvt.sckGpio, sgPrvt.sckPin);
    HAL_GPIO_DeInit(sgPrvt.misoGpio, sgPrvt.misoPin);
    HAL_GPIO_DeInit(sgPrvt.mosiGpio, sgPrvt.mosiPin);
    CAR_UNREFERRENCED(pSpi);
}

static int32_t carNrfSpiInit(void)
{
    int32_t rc = CAR_OK;
    sgCarNrfSpi.Instance = sgPrvt->spi;
    sgCarNrfSpi.Init.Mode = SPI_MODE_MASTER;
    sgCarNrfSpi.Init.Direction = SPI_DIRECTION_2LINES;
    sgCarNrfSpi.Init.DataSize = SPI_DATASIZE_8BIT;
    sgCarNrfSpi.Init.CLKPolarity = SPI_POLARITY_LOW;
    sgCarNrfSpi.Init.CLKPhase = SPI_PHASE_1EDGE;
    sgCarNrfSpi.Init.NSS = SPI_NSS_SOFT;
    sgCarNrfSpi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    sgCarNrfSpi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    sgCarNrfSpi.Init.TIMode = SPI_TIMODE_DISABLE;
    sgCarNrfSpi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    sgCarNrfSpi.Init.CRCPolynomial = 10;
    if (HAL_SPI_RegisterCallback(&sgCarNrfSpi, HAL_SPI_MSPINIT_CB_ID, carNrfSpiMspInit) != HAL_OK) {
        CAR_LOG_ERROR("Register func carNrfSpiMspInit failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }
    if (HAL_SPI_RegisterCallback(&sgCarNrfSpi, HAL_SPI_MSPDEINIT_CB_ID, carNrfSpiDeInit) != HAL_OK) {
        CAR_LOG_ERROR("Register func carNrfSpiDeInit failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }

    if (HAL_SPI_Init(&sgCarNrfSpi) != HAL_OK) {
        CAR_LOG_ERROR("Car nrf spi failed.");
        rc = -CAR_HAL_ERR;
        goto l_end;
    }

    __HAL_SPI_ENABLE(&sgCarNrfSpi);

    CAR_LOG_INFO("Nrf spi init ok.");

l_end:
    return rc;
}

static uint8_t carNrfSpiRwByte(SPI_HandleTypeDef *pSpi, uint8_t txData)
{
    uint8_t rxData;
    HAL_SPI_TransmitReceive(pSpi, &txData, &rxData, 1, 1000);
    return rxData
}

static void carNrfCe()
{

}

static void carNrfCsn()
{
    
}

static int32_t carNrfGpioInit(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    gpio_init_struct.Pin = sgPrvt.cePin;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(sgPrvt.ceGpio, &gpio_init_struct);

    gpio_init_struct.Pin = sgPrvt.csnPin;
    HAL_GPIO_Init(sgPrvt.csnGpio, &gpio_init_struct);

    gpio_init_struct.Pin = sgPrvt.csnPin;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(sgPrvt.irqGpio, &gpio_init_struct);

    NRF24L01_CE(0);
    NRF24L01_CSN(1);
    return CAR_OK;
}

static CarNrfHw_t sgNrfHw = {
    .pPrvt = &sgPrvt,
    .pfInit = carNrfSpiInit,
    .pfSpeedSet = NULL,
};

CarNrfHw_t *CarNrfHwGet()
{
    return &sgNrfHw;
}
