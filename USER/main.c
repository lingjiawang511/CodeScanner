#include"HeadType.h"	
#include "usart.h"
#include "TIM.h"
/***************修改记录1************************/
//1.修改扫描头扫描时间，由原来的扫描Ns,暂停Ns,修改为扫描Ns,暂停N/2s。
//2.修改通讯协议控制LED，由原来的方式：上位机点亮LED后，程序隔NS自动熄灭。修改为熄灭时间由上位机控制
//3.修改超时扫描时间逻辑，由原来的蜂鸣器响完后自动接在上一逻辑扫描，修改为蜂鸣器响完之后马上开始扫描
/***************修改记录1************************/


void delay_ms1(u16 ms)
{
	u16  i;
	u32  j;
	for(i=ms;i>0;i--)
	{
		for(j=5000;j>0;j--)
		;
	}
}
//配置时钟在target宏定义
int main(void)
{
		delay_init();
		LED_GPIO_Config();
		BEEP_GPIO_Config();
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		Scanner_GPIO_Config();
    USART1_Config();
    USART2_Config();
	  USART2_Puts("12345678");
	  USART2_Puts("12345678"); 
    TIM2_Config();
    TIM3_Config();
		TIM4_Config();
		delay_ms(1000);
    while(1){
        Dispose_Data_For_Host();
        Respond_Host_Comm();
				if(ERR_Beep_State){
					if(Beep_Num == 0){
						TIM_Cmd(TIM4, DISABLE );    //关闭定时器4
						ERR_Beep_State = 0;	
					}
				}
    }
        
}




