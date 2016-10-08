#ifndef __MQTTCONFIG_H_
#define __MQTTCONFIG_H_
#include "stm32f10x.h"



int transport_sendPacketBuffer(uint8_t* buf, int32_t buflen);
void mqtt_config(void);
void NVIC_Configuration0(void);
void TIM_Config(void);


#endif
