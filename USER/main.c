#include"HeadType.h"	
#include "usart.h"
#include "TIM.h"


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
//����ʱ����target�궨��
int main(void)
{
		delay_init();
		LED_GPIO_Config();
		BEEP_GPIO_Config();
		Scanner_GPIO_Config();
    USART1_Config();
    USART2_Config();
		delay_ms(1000);
		delay_ms(1000);
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
						TIM_Cmd(TIM4, DISABLE );    //�رն�ʱ��4
						ERR_Beep_State = 0;	
					}
				}
    }
        
}




