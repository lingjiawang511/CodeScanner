#include "beep.h"
#include"HeadType.h"

u16 Beep_Time;
u8 Beep_Num;
u8 ERR_Beep_State;
//=============================================================================
//函数名称:BEEP_GPIO_Config
//功能概要:蜂鸣器引脚配置
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
void BEEP_GPIO_Config(void)
{	
	//定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//使能GPIOC的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	///设置引脚模式为推免输出模式			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//设置引脚速度为50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//调用库函数，初始化GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	ERR_Beep_State = 0;
	Beep_Num = 0;
	Beep_Time = 1000;
}
//=============================================================================
//函数名称:Beep
//功能概要:使用延时方式让蜂鸣器响
//参数名称:retry：蜂鸣器响的次数
//函数返回:无
//注意    :此函数需要在5MS定时器2中断中调用
//=============================================================================
void Beep(u16 retry)
{
	u8 i;
 	while(retry--){
		for(i=0;i <50;i++){
			BEEP_ON;
			delay_ms(1);
			BEEP_OFF;
			delay_ms(1);
		}
		delay_ms(300);
	}
}
//=============================================================================
//函数名称:Beep_LoudNum
//功能概要:蜂鸣器响的次数，Beep_Num是全局变量
//参数名称:无
//函数返回:无
//注意    :此函数需要在5MS定时器2中断中调用
//=============================================================================
void Beep_LoudNum(void )
{
	static u8 retry = 60; //蜂鸣器响之后的间隔60*5ms=300ms
	if(Beep_Time){
		return;
	}
	if(Beep_Num > 0){
		retry--;
		if(retry <= 0){//蜂鸣器时间间隔
			Beep_Num--;	  	//蜂鸣器响的次数
			Beep_Time = 1000;//相当于五十个脉冲 100*2ms=200ms
			retry = 60;	
		}
	}	
}
//=============================================================================
//函数名称:Beep_Response
//功能概要:蜂鸣器响，使用定时器来实现
//参数名称:无
//函数返回:无
//注意    :此函数需要在2MS定时器3中断中调用
//=============================================================================

void Beep_Response(void )
{
	static u8 beep_static=0;
	if(Beep_Time >0){		
		if(beep_static){
			BEEP_OFF;
			beep_static = 0;
		}else{
		 	BEEP_ON;
			beep_static = 1;
		}
		Beep_Time--;
	}	
}



