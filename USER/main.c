#include"HeadType.h"	
#include "usart.h"
#include "TIM.h"
/***************�޸ļ�¼1************************/
//1.�޸�ɨ��ͷɨ��ʱ�䣬��ԭ����ɨ��Ns,��ͣNs,�޸�Ϊɨ��Ns,��ͣN/2s��
//2.�޸�ͨѶЭ�����LED����ԭ���ķ�ʽ����λ������LED�󣬳����NS�Զ�Ϩ���޸�ΪϨ��ʱ������λ������
//3.�޸ĳ�ʱɨ��ʱ���߼�����ԭ���ķ�����������Զ�������һ�߼�ɨ�裬�޸�Ϊ����������֮�����Ͽ�ʼɨ��
/***************�޸ļ�¼1************************/


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
						TIM_Cmd(TIM4, DISABLE );    //�رն�ʱ��4
						ERR_Beep_State = 0;	
					}
				}
    }
        
}




