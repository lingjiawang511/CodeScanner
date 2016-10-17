#include "codescanner.h"
#include"HeadType.h"


Host_COMM_Union_Type Host_Control;
Host_Answer_Type 	 Host_Answer;

//=============================================================================
//函数名称:Scanner_GPIO_Config
//功能概要:Scanner引脚配置
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
void Scanner_GPIO_Config(void)
{	/**PBO,PBI,PB6,output**/
	//定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//使能GPIOC的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6;
	///设置引脚模式为推免输出模式			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//设置引脚速度为50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//调用库函数，初始化GPIO
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/**PB5,PB7,input**/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	///设置引脚模式为推免输出模式			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	//设置引脚速度为50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//调用库函数，初始化GPIO
   	GPIO_Init(GPIOB, &GPIO_InitStructure);

	Scanner_Trigger_High;
	Scanner_Polarity_High;
	Scanner_Wakeup_Low;
	delay_ms(100);
	Scanner_Wakeup_High;
	//test
	Host_Control.control.scanner_time = 2*3;
	Host_Answer.answer_state = 0;
	Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
}
//=============================================================================
//函数名称:scanner_scan
//功能概要:循环扫描时间定时，在定时器调用
//参数名称:无
//函数返回:无
//注意    :定时器扫描时间为5ms,所以实际扫描头扫描时间为N*5*100
//=============================================================================
void scanner_scan(void)
{	
	static u16 scan_time = 600;
	static u16 scan_time_old = 600;
	static u8  trigger_static = 0;

	if(Host_Answer.answer_state == 2){//第一时间先响应参数修改
		if(Host_Control.control.scanner_time <0X02){
			Host_Control.control.scanner_time = 0X02;
		}
		scan_time_old =Host_Control.control.scanner_time * 100;
		scan_time =	scan_time_old;
		Host_Answer.answer_state = 1;//修改之后变为主机响应状态
		Host_Answer.answer_timeout = ANSWER_SCANTIME;
	}
	if(Host_Answer.answer_state == 1){//主机响应了，就不以固定的扫描周期扫描了
		if(Host_Answer.answer_timeout <=0){
			Host_Answer.answer_state = 0;
			Host_Answer.start_timeout = 0;
		}
		if(Host_Answer.start_timeout ==1){
		    if(Host_Answer.answer_timeout == ANSWER_SCANTIME){
		 	 	Scanner_Trigger_Low;//主机响应状态
			 }
		 Host_Answer.answer_timeout--;	//此处相当于产生脉冲后一直为低
		 }else if(Host_Answer.start_timeout ==2){		 	 	
				if(Host_Answer.Nanswer_timeout <=0){//上位机超时没有回应，当错误处理
					Rled_Num = 5;	//错误处理
					Beep_Num = 3;
					TIM_Cmd(TIM4, ENABLE );
					ERR_Beep_State = 1;
					Host_Answer.answer_state = 0;
					trigger_static = 1;	//超时完成第一时间先扫描
					Scanner_Trigger_High;//超时完成第一时间先扫描，脉冲驱动
					
					Host_Control.control.gled_state = 0;
					Gled_Light_Static = 0;
					Gled_Light_Time = 0;
					Gled_Num = 0;
					GLED_OFF;
					Host_Control.control.rled_state = 0;
					Rled_Light_Static = 0;
					Rled_Light_Time = 0;
				}
				Host_Answer.Nanswer_timeout--;
		 }
		return;
	}
	if(scan_time >0){
		scan_time--;
	}
	if(Beep_Num == 0){//蜂鸣器响的时候不能扫描	
		if(scan_time <=0){
			if(trigger_static){
				Scanner_Trigger_Low;
				trigger_static = 0;
				scan_time =scan_time_old;
			}else{
			 	Scanner_Trigger_High;
				trigger_static = 1;
				scan_time =scan_time_old/2;//高电平的时间是低电平的1/2，扫面时间是休眠时间的2倍
			}
//			scan_time =scan_time_old;		
		}
   }
}



