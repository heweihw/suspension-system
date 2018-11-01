#ifndef __ENCODER_H__
#define __ENCODER_H__
#include "sys.h"
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void TIM4_Int_Init(u16 arr,u16 psc);
#endif