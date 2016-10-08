#ifndef __CODESCANNER_H
#define __CODESCANNER_H

#include "stm32f10x.h"

#define       Scanner_Trigger_High   	GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define       Scanner_Trigger_Low 	 	GPIO_ResetBits(GPIOB, GPIO_Pin_1)

#define       Scanner_Polarity_High   	GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define       Scanner_Polarity_Low  	GPIO_ResetBits(GPIOB, GPIO_Pin_0)

#define       Scanner_Wakeup_High   	GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define       Scanner_Wakeup_Low  		GPIO_ResetBits(GPIOB, GPIO_Pin_1)

void Scanner_GPIO_Config(void);
void scanner_scan(void);
#endif

