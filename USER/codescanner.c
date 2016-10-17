#include "codescanner.h"
#include"HeadType.h"


Host_COMM_Union_Type Host_Control;
Host_Answer_Type 	 Host_Answer;

//=============================================================================
//��������:Scanner_GPIO_Config
//���ܸ�Ҫ:Scanner��������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void Scanner_GPIO_Config(void)
{	/**PBO,PBI,PB6,output**/
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//ѡ��Ҫ�õ�GPIO����		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6;
	///��������ģʽΪ�������ģʽ			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//���������ٶ�Ϊ50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//���ÿ⺯������ʼ��GPIO
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/**PB5,PB7,input**/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	///��������ģʽΪ�������ģʽ			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	//���������ٶ�Ϊ50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//���ÿ⺯������ʼ��GPIO
   	GPIO_Init(GPIOB, &GPIO_InitStructure);

	Scanner_Trigger_High;
	Scanner_Polarity_High;
	Scanner_Wakeup_Low;
	delay_ms(50);
	Scanner_Wakeup_High;
	delay_ms(50);
	Scanner_Trigger_Low;
	//test
	Host_Control.control.scanner_time = 2*3;
	Host_Answer.answer_state = 0;
	Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
}
//=============================================================================
//��������:scanner_scan
//���ܸ�Ҫ:ѭ��ɨ��ʱ�䶨ʱ���ڶ�ʱ������
//��������:��
//��������:��
//ע��    :��ʱ��ɨ��ʱ��Ϊ5ms,����ʵ��ɨ��ͷɨ��ʱ��ΪN*5*100
//=============================================================================
void scanner_scan(void)
{	
	static u16 scan_time = DEFAULT_SCANTIME;					//Ĭ��ɨ������ʱ��
	static u16 scan_time_old = DEFAULT_SCANTIME;
	static u8  trigger_static = 0;

	if(Host_Answer.answer_state == 2){//��һʱ������Ӧ�����޸�,��λ������ɨ��ͷɨ��ʱ��
		if(Host_Control.control.scanner_time <0X02){
			Host_Control.control.scanner_time = 0X02;
		}
		scan_time_old =Host_Control.control.scanner_time * 100;
		scan_time =	scan_time_old;
		Host_Answer.answer_state = 1;//�޸�֮���Ϊ������Ӧ״̬
		Host_Answer.answer_timeout = ANSWER_SCANTIME;
	}
	if(Host_Answer.answer_state == 1){//������Ӧ�ˣ��Ͳ��Թ̶���ɨ������ɨ���ˣ�ɨ�赽��ά�룬һֱɨ�赽����ֵ����
		if(Host_Answer.answer_timeout <=0){
			Host_Answer.answer_state = 0;
			Host_Answer.start_timeout = 0;
		}
		if(Host_Answer.start_timeout ==1){
		    if(Host_Answer.answer_timeout == ANSWER_SCANTIME){
		 	 	Scanner_Trigger_Low;//������Ӧ״̬
			 }
		 Host_Answer.answer_timeout--;	//�˴��൱�ڲ��������һֱΪ��
		 }else if(Host_Answer.start_timeout ==2){		 	 	
				if(Host_Answer.Nanswer_timeout <=0){//��λ����ʱû�л�Ӧ����������
					Rled_Num = 5;	//������
					Beep_Num = 3;
					TIM_Cmd(TIM4, ENABLE );
					ERR_Beep_State = 1;
					Host_Answer.answer_state = 0;
					trigger_static = 1;	//��ʱ��ɵ�һʱ����ɨ��
					Scanner_Trigger_High;//��ʱ��ɵ�һʱ����ɨ�裬��������
					
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
	if(Beep_Num == 0){//���������ʱ����ɨ��	
		if(scan_time <=0){
			if(trigger_static){
				Scanner_Trigger_Low;
				trigger_static = 0;
				scan_time =scan_time_old;
			}else{
			 	Scanner_Trigger_High;
				trigger_static = 1;
				scan_time =scan_time_old/2;//�ߵ�ƽ��ʱ���ǵ͵�ƽ��1/2��ɨ��ʱ��������ʱ���2��
			}
//			scan_time =scan_time_old;		
		}
   }
}



