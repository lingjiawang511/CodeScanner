/****************************************Copyright (c)****************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:               ads1232.h
** Descriptions:            None
**
**--------------------------------------------------------------------------------------------------------
** Created by:              ling
** Created date:            2015-8-22
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef _ADS1232_H_
#define _ADS1232_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>


//#define DACTEST     1

#define AD_DOUT_PC11        GPIO_Pin_11     
#define AD_SCK_PC12         GPIO_Pin_12     
#define AD_DOWN_PC13        GPIO_Pin_13  
#if DACTEST  
#define AD_SPEED_PC7        GPIO_Pin_7     
#else
#define AD_SPEED_PB3        GPIO_Pin_3 
#endif

//#define Set_AD_DOUT         GPIO_SetBits(GPIOC,AD_DOUT_PC11);  
#define Set_AD_SCK          GPIO_SetBits(GPIOC,AD_SCK_PC12);  
#define Set_AD_DOWN         GPIO_SetBits(GPIOC,AD_DOWN_PC13); 
#if DACTEST
#define Set_AD_SPEED        GPIO_SetBits(GPIOC,AD_SPEED_PC7);  
#else
#define Set_AD_SPEED        GPIO_SetBits(GPIOB,AD_SPEED_PB3); 
#endif
//#define Reset_AD_DOUT       GPIO_ResetBits(GPIOC,AD_DOUT_PC11);  
#define Reset_AD_SCK        GPIO_ResetBits(GPIOC,AD_SCK_PC12);  
#define Reset_AD_DOWN       GPIO_ResetBits(GPIOC,AD_DOWN_PC13);
#if DACTEST 
#define Reset_AD_SPEED      GPIO_ResetBits(GPIOC,AD_SPEED_PC7); 
#else
#define Reset_AD_SPEED      GPIO_ResetBits(GPIOB,AD_SPEED_PB3);
#endif
#define Read_AD_DOUT        GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)  //返回的是一个字节

#define Delay_us	20
#define	Delay_ms	1000

/* Private function prototypes -----------------------------------------------*/
void delay(uint16_t us);
void ADS1232_GPIO_Init(void);
extern  uint32_t AD_read(uint8_t Adjust);

void InitFilter(long dat);
long abs(long a,long b);
void Filter(long Newdat);
/* Private variables ---------------------------------------------------------*/
extern FunctionalState TimeDisplay;
extern unsigned char FlagADC;
extern UL ADS1232_Valu;
extern long FilterOut;
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
