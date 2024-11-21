#ifndef __CAR_NRF24L02_H__
#define __CAR_NRF24L02_H__

typedef struct CarNrfHw
{
    struct CarNrfPrvt *pPrvt;
    int32_t (*pfInit)(void);
    void (*pfSpeedSet)(struct CarNrfHw *pHw, uint8_t speed);
    uint8_t (*pfRwByte)(struct CarNrfHw *pHw, uint8_t txData);
} CarNrfHw_t

CarNrfHw_t *CarNrfHwGet(void);

#endif