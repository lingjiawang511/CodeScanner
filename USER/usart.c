#include"HeadType.h"
#include "usart.h"

#define USART1REMAP 0
/*�Զ���֡��ʱ��ʱʱ��*/
#define AUTO_FRAME_TIMEOUT1  100  //10*2ms
#define AUTO_FRAME_TIMEOUT2  50  //10*2ms
/*�Զ���֡��ʱ��ʱʱ��*/

Usart_Type Usart1_Control_Data;
Usart_Type Usart2_Control_Data;
char Auto_Frame_Time1;
char Auto_Frame_Time2;

//=============================================================================
//��������:Init_USART1
//���ܸ�Ҫ:USART1 ��ʼ��������ز���
//����˵��:��
//��������:��
//=============================================================================
static void Init_USART1(void)
{
    Usart1_Control_Data.tx_index = 0;
    Usart1_Control_Data.rx_index = 0;
    Usart1_Control_Data.tx_count = 0;
    Usart1_Control_Data.rx_count = 0;
    Usart1_Control_Data.rx_start = 0;
    Usart1_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//��������:Init_USART2
//���ܸ�Ҫ:USART2 ��ʼ��������ز���
//����˵��:��
//��������:��
//=============================================================================
static void Init_USART2(void)
{
    Usart2_Control_Data.tx_index = 0;
    Usart2_Control_Data.rx_index = 0;
    Usart2_Control_Data.tx_count = 0;
    Usart2_Control_Data.rx_count = 0;
    Usart2_Control_Data.rx_start = 0;
    Usart2_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//��������:USART1_Interrupts_Config
//���ܸ�Ҫ:USART1 �ж����ȼ�����
//����˵��:��
//��������:��
//=============================================================================
static void USART1_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//��������:USART2_Interrupts_Config
//���ܸ�Ҫ:USART2 �ж����ȼ�����
//����˵��:��
//��������:��
//=============================================================================
static void USART2_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//��������:USART1_Config
//���ܸ�Ҫ:USART1 ��ʼ������,����ģʽ���á�57600 8-N-1
//����˵��:��
//��������:��
//=============================================================================
#if USART1REMAP  //���USART1ʹ����ӳ��

void USART1_Config(void )
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);

  /*  USART1_TX -> PB6 */
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	       //ѡ�д���Ĭ������ܽ�         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
  GPIO_Init(GPIOB, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��	
  
  /*  USART1_RX ->	PB7*/

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  USART_InitStructure.USART_BaudRate = 19200; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

  USART_Init(USART1, &USART_InitStructure); //�����ϸ���ֵ�Ľṹ�����⺯��USART_Init���г�ʼ��
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    /* Enable the EXTI Interrupt */
  USART_Cmd(USART1, ENABLE);//����USART1��ע��������RCC_APB2PeriphClockCmd()���õ�����
  USART1_Interrupts_Config();
  Init_USART1();
}
#else
void USART1_Config(void)
{
	//gpio��usart��ʼ���ṹ�嶨��
	//��gpio����ʱ����������ʱ������Ҫ��ʼ��ͬʱgpio��ppp���裬��������ʹ�ܺͿ���ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//����gpio������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	//gpio��ʼ���ṹ���Ա��ֵ
	//��ʼ��PA��9�������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//ִ�������gpio��ʼ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA.10��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART��ʼ��
	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	/* Enable the EXTI Interrupt */
	USART_Cmd(USART1, ENABLE);//����USART1��ע��������RCC_APB2PeriphClockCmd()���õ�����
	
	USART1_Interrupts_Config();
	Init_USART1();
}

#endif
 //=============================================================================
//��������:USART2_Config
//���ܸ�Ҫ:USART2 ��ʼ������,����ģʽ���á�57600 8-N-1
//����˵��:��
//��������:��
//=============================================================================
void USART2_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable the USART2 Pins Software Remapping */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 

	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	/* Enable the USART2 Interrupt */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ���жϷ���
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);    

	USART_InitStructure.USART_BaudRate = 19200; 			 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	   
	USART_InitStructure.USART_Parity = USART_Parity_No; 	  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);
	USART2_Interrupts_Config();
	Init_USART2();
}

//=============================================================================
//��������:fputc
//���ܸ�Ҫ:�ض���c�⺯��printf��USART
//����˵��:��
//��������:��
//ע��   :��printf����,��Ҫ��ѡUse Microlib
//=============================================================================

int fputc(int ch, FILE *f)
{
//��Printf���ݷ������� 
  USART_SendData(USART1, (unsigned char) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
  return (ch);
}
//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��   :��
//=============================================================================
void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}
//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��   :��
//=============================================================================
void USART2_Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART2_Puts(char * str)
{
    while(*str){
        USART_SendData(USART2, *str++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}
//=============================================================================
//��������:USART1_Do_Tx
//���ܸ�Ҫ:������һ�����ݷ��ͳ�ȥ��ͨ���жϷ���
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Do_Tx(void )
{
    if (Usart1_Control_Data.tx_index < Usart1_Control_Data.tx_count) {
		USART_SendData(USART1, Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index]);
		Usart1_Control_Data.tx_index++;
	}else{
       Usart1_Control_Data.tx_count = 0; 
       Usart1_Control_Data.tx_index = 0;
    }
}
//=============================================================================
//��������:USART2_Do_Tx
//���ܸ�Ҫ:�����ڶ������ݷ��ͳ�ȥ��ͨ���жϷ���
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART2_Do_Tx(void )
{
    if (Usart2_Control_Data.tx_index < Usart2_Control_Data.tx_count) {
		USART_SendData(USART2, Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index]);
		Usart2_Control_Data.tx_index++;
	}else{
       Usart2_Control_Data.tx_count = 0; 
       Usart2_Control_Data.tx_index = 0;	   
    }
}
//=============================================================================
//��������:USART1_Do_Rx
//���ܸ�Ҫ:������һ���յ�����
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Do_Rx(u8 rxdata)
{       
    if (0 == Usart1_Control_Data.rx_aframe){
       if (0 == Usart1_Control_Data.rx_index){  //���յ�һ֡�ĵ�һ�����ݿ�����ʱ��3��ʱ���Զ���֡����
           Usart1_Control_Data.rx_start = 1;
           Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
       }else {
            if (Auto_Frame_Time1 <=0){   //ʱ�䳬ʱ�Զ���֡
               Usart1_Control_Data.rx_aframe = 1; 
               Usart1_Control_Data.rx_start = 0;
               Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index;
               Usart1_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
               return ;   //ֱ�ӷ��ز���������              
            }                    
        }
       	Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index] = rxdata;
        Usart1_Control_Data.rx_index++;
        if (Usart1_Control_Data.rx_index > (RxBufMax - 1)){
            Usart1_Control_Data.rx_index = (RxBufMax - 1);
            Usart1_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
            Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index+1;
            Usart1_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
            Usart1_Control_Data.rx_start = 0;
            Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        }   
    }else{  //���յ�һ֡���ݺ���봦����ſ��Լ���������һ֡����
       Usart1_Control_Data.rx_start = 0;
       Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        return;
    }           
}
//=============================================================================
//��������:USART2_Do_Rx
//���ܸ�Ҫ:�����ڶ����յ�����
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART2_Do_Rx(u8 rxdata)
{       
    if (0 == Usart2_Control_Data.rx_aframe){
       if (0 == Usart2_Control_Data.rx_index){  //���յ�һ֡�ĵ�һ�����ݿ�����ʱ��3��ʱ���Զ���֡����
           Usart2_Control_Data.rx_start = 1;
           Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
       }else {
            if (Auto_Frame_Time2 <=0){   //ʱ�䳬ʱ�Զ���֡
               Usart2_Control_Data.rx_aframe = 1; 
               Usart2_Control_Data.rx_start = 0;
               Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index;
               Usart2_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
               return ;   //ֱ�ӷ��ز���������              
            }                    
        }
       	Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_index] = rxdata;
        Usart2_Control_Data.rx_index++;
        if (Usart2_Control_Data.rx_index > (RxBufMax - 1)){
            Usart2_Control_Data.rx_index = (RxBufMax - 1);
            Usart2_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
            Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index+1;
            Usart2_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
            Usart2_Control_Data.rx_start = 0;
            Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        }   
    }else{  //���յ�һ֡���ݺ���봦����ſ��Լ���������һ֡����
       Usart2_Control_Data.rx_start = 0;
       Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        return;
    }           
}
//=============================================================================
//��������:Execute_Host_Comm
//���ܸ�Ҫ:ִ����λ������������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
static u8 Execute_Host_Comm(void)
{
	u8 i;
	u16 crc;
	if(Usart1_Control_Data.rx_count != 16){
		return 0;
	}
	crc=CRC_GetCCITT(Usart1_Control_Data.rxbuf,Usart1_Control_Data.rx_count-2);
	if((Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-1]+\
 	    Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-2]*256 == crc)){	    
		for(i = 0;i < 16;i++){
            Host_Control.host_buf[i] = Usart1_Control_Data.rxbuf[i];
        }//�����ݸ��Ƹ�����ͨѶ�ṹ��
		switch(Host_Control.control.beep_state){
		case 0x00:	 
					 break;
		case 0x01: 	 Host_Control.control.beep_state = 0;
					 break;
		case 0x02: 	 Beep_Num = Host_Control.control.beep_num;
					 Host_Control.control.beep_state = 0;
					 break;
		default : break;
		}
		switch(Host_Control.control.gled_state){
		case 0x00: 
					break;
		case 0x01: 	GLED_OFF;
					Host_Control.control.gled_state = 0;
					Gled_Light_Static = 0;
					Gled_Light_Time = 0;
					break;
		case 0x02: 	GLED_ON;
					Gled_Light_Static = 1;
					Gled_Light_Time = Host_Control.control.gled_num * 200;	//5ms*200=1S
					Host_Control.control.gled_state = 0;
					break;
	    case 0x03: 	Gled_Num = Host_Control.control.gled_num;
					Host_Control.control.gled_state = 0;
					Gled_Light_Static = 0;
					Gled_Light_Time = 0;
					break;
		default : break;
		}
		switch(Host_Control.control.rled_state){
		case 0x00: 
					break;
		case 0x01: 	RLED_OFF;
					Host_Control.control.rled_state = 0;
					Rled_Light_Static = 0;
					Rled_Light_Time = 0;
					break;
		case 0x02: 	RLED_ON;
					Rled_Light_Static = 1;
					Rled_Light_Time = Host_Control.control.rled_num * 200;	//5ms*200=1S
					Host_Control.control.rled_state = 0;
					break;
	    case 0x03: 	Rled_Num = Host_Control.control.rled_num;
					Host_Control.control.rled_state = 0;
					Rled_Light_Static = 0;
					Rled_Light_Time = 0;
					break;
		default : break;
		}
		switch(Host_Control.control.scanner_state){
		case 0x00: 
					break;
		case 0x01: 	Host_Answer.answer_state = Host_Control.control.scanner_state;
					Host_Control.control.scanner_state = 0;
					break;
		case 0x02: 	Host_Answer.answer_state = Host_Control.control.scanner_state;
					Host_Control.control.scanner_state = 0;
					break;
		default : break;
		}
		return 1;	
	}else{
		 return 0;
	}
}
//=============================================================================
//��������:response_reader
//���ܸ�Ҫ:��Ӧɨ��ͷ���ݰ�����
//����˵��:��
//��������:����1��������У����ȷ������0��������У�����
//ע��    :��ɨ��ͷ�����ݣ�ʹ��CRC14У�鲢��ֻУ������λ����У������ͷ
//=============================================================================
#if SCAN_CR8000
static u8 response_reader(void)
{	   	
	u16 crc;
	if(Usart2_Control_Data.rx_count < 16){
		return 0;
	}
	crc=CRC_GetCCITT(Usart2_Control_Data.rxbuf,Usart2_Control_Data.rx_count-2);
	if((Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-1]+\
 	    Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-2]*256 == crc)){
		/**CRCУ��OK����ӦREADER�ź� 'Y'**/
		Usart2_Control_Data.tx_count = 0;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 'Y' ;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x01;  //data size 
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = Usart2_Control_Data.rxbuf[8];  
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;  //data end
		crc=CRC_GetCCITT(&Usart2_Control_Data.txbuf[4],Usart2_Control_Data.tx_count-4);
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = (crc>>8)&0x7F; //CRC14
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = crc&0x7F;
		/**��������**/
//			Beep_Num = 5;
//			Gled_Num = 5;
//			Rled_Num = 5;
//			Host_Answer.answer_state = 1;
//			Host_Answer.answer_timeout = ANSWER_SCANTIME;
		Host_Answer.answer_state = 1; //ɨ��ͷ���յ���ȷ���ݺ�Ͱ�״̬ת��
		Host_Answer.start_timeout =2; //���Ϳ�ʼ����λ�����ճ�ʱʱ��
		Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
		Host_Answer.answer_timeout = ANSWER_SCANTIME;
		Usart2_Control_Data.tx_index = 0;
		USART_SendData(USART2,Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index++]);
		return 1;
	}else{
		 /**CRCУ��NOK����ӦREADER�ź� 'R'**/
		Usart2_Control_Data.tx_count = 0;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 'R' ;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;  //data size less than 32
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x05;  //the Reader will use its preferred maximum packet size
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;  //data end
		crc=CRC_GetCCITT(&Usart2_Control_Data.txbuf[4],Usart2_Control_Data.tx_count);
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = (crc>>8)&0x7F;	//CRC14
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = crc&0x7F;
		/**��������**/
		Usart2_Control_Data.tx_index = 0;
		USART_SendData(USART2,Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index++]);
	
		return 0;
	}	
}
#else
static u8 response_reader(void)
{	   	
	if(Usart2_Control_Data.rx_count < 3){
		return 0;
	}else{
		Host_Answer.answer_state = 1; //ɨ��ͷ���յ���ȷ���ݺ�Ͱ�״̬ת��
		Host_Answer.start_timeout =2; //���Ϳ�ʼ����λ�����ճ�ʱʱ��
		Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
		Host_Answer.answer_timeout = ANSWER_SCANTIME;
		return 1;
   	}
}
#endif
//=============================================================================
//��������:Copy_Scanner_Data
//���ܸ�Ҫ:��ɨ�����õ������ݸ��Ƶ�����һ
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
#if SCAN_CR8000
static void  Copy_Scanner_Data(void)
{
    u8 i;
    for(i = 0;i < Usart2_Control_Data.rx_count;i++){
        Usart1_Control_Data.txbuf[i] = Usart2_Control_Data.rxbuf[i];
    }
    Usart1_Control_Data.tx_count = Usart2_Control_Data.rx_count;

    Usart2_Control_Data.rx_count = 0;
    Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2;
    
    Usart1_Control_Data.rx_index = 0;
    Usart1_Control_Data.tx_index = 0;
    USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
//        return Usart2_Control_Data.rx_count;
}
#else

static void  Copy_Scanner_Data(void)
{
  u8 i,j;
	u16 crc;
	j=0;
	Usart1_Control_Data.txbuf[j++] = 0x01;	
	Usart1_Control_Data.txbuf[j++] = 0x58;
	Usart1_Control_Data.txbuf[j++] = 0x52;	
	Usart1_Control_Data.txbuf[j++] = 0x31;
	Usart1_Control_Data.txbuf[j++] = 0x35;	
	Usart1_Control_Data.txbuf[j++] = 0x36;
	Usart1_Control_Data.txbuf[j++] = 0x37;	
	Usart1_Control_Data.txbuf[j++] = 0x38;
	Usart1_Control_Data.txbuf[j++] = 0x01;	
	Usart1_Control_Data.txbuf[j++] = 0xFF;
	Usart1_Control_Data.txbuf[j++] = 0xFF;	
	Usart1_Control_Data.txbuf[j++] = 0xFF;
	Usart1_Control_Data.txbuf[j++] = 0xFF;
	Usart1_Control_Data.txbuf[j++] = Usart2_Control_Data.rx_count >>8;	
	Usart1_Control_Data.txbuf[j++] = Usart2_Control_Data.rx_count;
    for(i = 0;i < Usart2_Control_Data.rx_count;i++){
        Usart1_Control_Data.txbuf[i+j] = Usart2_Control_Data.rxbuf[i];
    }
  Usart1_Control_Data.tx_count = Usart2_Control_Data.rx_count + j;
	crc=CRC_GetCCITT(Usart1_Control_Data.txbuf,Usart1_Control_Data.tx_count);
	Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc >> 8;
  Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc;

	Usart2_Control_Data.rx_count = 0;
  Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2;
    
  Usart1_Control_Data.rx_index = 0;
  Usart1_Control_Data.tx_index = 0;
  USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
//        return Usart2_Control_Data.rx_count;
}

#endif

//=============================================================================
//��������:Dispose_Data_For_Host
//���ܸ�Ҫ:��������ɨ���������������͵���λ��
//����˵��:��
//��������:��
//ע��    :������ȷ���ϴ�����λ��
//=============================================================================
void Dispose_Data_For_Host(void)
{

	if (1 == Usart2_Control_Data.rx_aframe){    
   		if(response_reader()){ 		
   			Copy_Scanner_Data();
		}else{
		//no use 	
		}
		Usart2_Control_Data.rx_aframe = 0;
   }
}
//=============================================================================
//��������:Respond_Host_Comm
//���ܸ�Ҫ:��Ӧ��λ���ķ�����������������Ѿ��Ӵ���һ��������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void Respond_Host_Comm(void)
{
//    u8 i=0;
    if (1 == Usart1_Control_Data.rx_aframe){    
        if(Execute_Host_Comm()){
//	        /*���Գ��򣬰ѵ��Է����������ݻط���ȥ*/
//	        for(i = 0;i < Usart1_Control_Data.rx_count;i++){
//	            Usart1_Control_Data.txbuf[i] = Usart1_Control_Data.rxbuf[i];
//	        }
//	        Usart1_Control_Data.tx_count =Usart1_Control_Data.rx_count;
//	        Usart1_Control_Data.tx_index = 0;
//	        USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
//	        /*���Գ��򣬰ѵ��Է����������ݻط���ȥ*/
	        Usart1_Control_Data.rx_count = 0;
	        Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1;
			if(Beep_Num){
				TIM_Cmd(TIM4, ENABLE );
			}
			while(Beep_Num){
			   	Host_Answer.start_timeout =0; //���������죬֤�����ᳬʱ,�����������̫�ö�����ִ�г�ʱ
					Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
			}  //����ȵ���Ӧ�꣬�ſ��Լ�����������
	    Scanner_Trigger_High;
			TIM_Cmd(TIM4, DISABLE );    //�رն�ʱ��4
			delay_ms(100);
			Host_Answer.start_timeout = 1;
			Host_Answer.answer_timeout = ANSWER_SCANTIME;
		}
	    Usart1_Control_Data.rx_aframe = 0;
    }else{
        return;
    }
}

