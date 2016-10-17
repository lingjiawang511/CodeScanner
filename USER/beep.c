#include "beep.h"
#include"HeadType.h"

u16 Beep_Time;
u8 Beep_Num;
u8 ERR_Beep_State;
//=============================================================================
//��������:BEEP_GPIO_Config
//���ܸ�Ҫ:��������������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void BEEP_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//ѡ��Ҫ�õ�GPIO����		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	///��������ģʽΪ�������ģʽ			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//���������ٶ�Ϊ50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//���ÿ⺯������ʼ��GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	ERR_Beep_State = 0;
	Beep_Num = 0;
	Beep_Time = BEEP_LOUD_TIME;
}
//=============================================================================
//��������:Beep
//���ܸ�Ҫ:ʹ����ʱ��ʽ�÷�������
//��������:retry����������Ĵ���
//��������:��
//ע��    :�˺�����Ҫ��5MS��ʱ��2�ж��е���
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
//��������:Beep_LoudNum
//���ܸ�Ҫ:��������Ĵ�����Beep_Num��ȫ�ֱ���
//��������:��
//��������:��
//ע��    :�˺�����Ҫ��5MS��ʱ��2�ж��е���
//=============================================================================
void Beep_LoudNum(void )
{
	static u8 retry = BEEP_PAUSE_TIME; //��������֮��ļ��60*5ms=300ms
	if(Beep_Time){
		return;
	}
	if(Beep_Num > 0){
		retry--;
		if(retry <= 0){//������ʱ����
			Beep_Num--;	  	//��������Ĵ���
			Beep_Time = BEEP_LOUD_TIME;//�൱��500������ 1000*0.2ms=200ms
			retry = BEEP_PAUSE_TIME;	
		}
	}	
}
//=============================================================================
//��������:Beep_Response
//���ܸ�Ҫ:�������죬ʹ�ö�ʱ����ʵ��
//��������:��
//��������:��
//ע��    :�˺�����Ҫ��2MS��ʱ��3�ж��е���
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



