#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"

#define       BEEP_ON   GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define       BEEP_OFF  GPIO_ResetBits(GPIOA, GPIO_Pin_6)

extern u8 Beep_Num;
extern u8 ERR_Beep_State;
void BEEP_GPIO_Config(void);
void Beep(u16 retry);
void Beep_LoudNum(void );
void Beep_Response(void );
#endif

