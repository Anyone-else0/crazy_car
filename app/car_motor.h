#ifndef __CAR_MOTOR_H__
#define __CAR_MOTOR_H__

typedef struct CarMotor
{
    TIM_HandleTypeDef mtTimHdl;
    TIM_HandleTypeDef turnMtHdl;


} CarMotor_t;

#endif