#include"HeadType.h"
#include "usart.h"

#define USART1REMAP 0
/*自动成帧超时定时时间*/
#define AUTO_FRAME_TIMEOUT1  100  //10*2ms
#define AUTO_FRAME_TIMEOUT2  50  //10*2ms
/*自动成帧超时定时时间*/

Usart_Type Usart1_Control_Data;
Usart_Type Usart2_Control_Data;
char Auto_Frame_Time1;
char Auto_Frame_Time2;

//=============================================================================
//函数名称:Init_USART1
//功能概要:USART1 初始化串口相关参数
//参数说明:无
//函数返回:无
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
//函数名称:Init_USART2
//功能概要:USART2 初始化串口相关参数
//参数说明:无
//函数返回:无
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
//函数名称:USART1_Interrupts_Config
//功能概要:USART1 中断优先级配置
//参数说明:无
//函数返回:无
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
//函数名称:USART2_Interrupts_Config
//功能概要:USART2 中断优先级配置
//参数说明:无
//函数返回:无
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
//函数名称:USART1_Config
//功能概要:USART1 初始化配置,工作模式配置。57600 8-N-1
//参数说明:无
//函数返回:无
//=============================================================================
#if USART1REMAP  //如果USART1使用了映射

void USART1_Config(void )
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);

  /*  USART1_TX -> PB6 */
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	       //选中串口默认输出管脚         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //定义输出最大速率 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚9的模式  
  GPIO_Init(GPIOB, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化	
  
  /*  USART1_RX ->	PB7*/

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  USART_InitStructure.USART_BaudRate = 19200; //波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚

  USART_Init(USART1, &USART_InitStructure); //将以上赋完值的结构体带入库函数USART_Init进行初始化
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    /* Enable the EXTI Interrupt */
  USART_Cmd(USART1, ENABLE);//开启USART1，注意与上面RCC_APB2PeriphClockCmd()设置的区别
  USART1_Interrupts_Config();
  Init_USART1();
}
#else
void USART1_Config(void)
{
	//gpio，usart初始化结构体定义
	//当gpio复用时（开启外设时），需要初始化同时gpio和ppp外设，并对他们使能和开启时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//开启gpio和外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	//gpio初始化结构体成员赋值
	//初始化PA。9，复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//执行上面的gpio初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA.10浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART初始化
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
	USART_Cmd(USART1, ENABLE);//开启USART1，注意与上面RCC_APB2PeriphClockCmd()设置的区别
	
	USART1_Interrupts_Config();
	Init_USART1();
}

#endif
 //=============================================================================
//函数名称:USART2_Config
//功能概要:USART2 初始化配置,工作模式配置。57600 8-N-1
//参数说明:无
//函数返回:无
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
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//选择中断分组
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
//函数名称:fputc
//功能概要:重定向c库函数printf到USART
//参数说明:无
//函数返回:无
//注意   :由printf调用,需要勾选Use Microlib
//=============================================================================

int fputc(int ch, FILE *f)
{
//将Printf内容发往串口 
  USART_SendData(USART1, (unsigned char) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
  return (ch);
}
//=============================================================================
//函数名称:USART1_Putc
//功能概要:将USART1_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意   :无
//=============================================================================
void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//函数名称:USART1_Putc
//功能概要:将USART1_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意    :无
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
//函数名称:USART1_Putc
//功能概要:将USART1_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意   :无
//=============================================================================
void USART2_Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//函数名称:USART1_Putc
//功能概要:将USART1_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART2_Puts(char * str)
{
    while(*str){
        USART_SendData(USART2, *str++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}
//=============================================================================
//函数名称:USART1_Do_Tx
//功能概要:将串口一的数据发送出去，通过中断发送
//参数说明:无
//函数返回:无
//注意    :无
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
//函数名称:USART2_Do_Tx
//功能概要:将串口二的数据发送出去，通过中断发送
//参数说明:无
//函数返回:无
//注意    :无
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
//函数名称:USART1_Do_Rx
//功能概要:处理串口一接收的数据
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART1_Do_Rx(u8 rxdata)
{       
    if (0 == Usart1_Control_Data.rx_aframe){
       if (0 == Usart1_Control_Data.rx_index){  //接收第一帧的第一个数据开启定时器3做时间自动成帧处理
           Usart1_Control_Data.rx_start = 1;
           Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
       }else {
            if (Auto_Frame_Time1 <=0){   //时间超时自动成帧
               Usart1_Control_Data.rx_aframe = 1; 
               Usart1_Control_Data.rx_start = 0;
               Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index;
               Usart1_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
               return ;   //直接返回不接受数据              
            }                    
        }
       	Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index] = rxdata;
        Usart1_Control_Data.rx_index++;
        if (Usart1_Control_Data.rx_index > (RxBufMax - 1)){
            Usart1_Control_Data.rx_index = (RxBufMax - 1);
            Usart1_Control_Data.rx_aframe = 1;  //接收数据长度自动成帧
            Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index+1;
            Usart1_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
            Usart1_Control_Data.rx_start = 0;
            Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        }   
    }else{  //接收到一帧数据后必须处理完才可以继续接受下一帧数据
       Usart1_Control_Data.rx_start = 0;
       Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        return;
    }           
}
//=============================================================================
//函数名称:USART2_Do_Rx
//功能概要:处理串口二接收的数据
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART2_Do_Rx(u8 rxdata)
{       
    if (0 == Usart2_Control_Data.rx_aframe){
       if (0 == Usart2_Control_Data.rx_index){  //接收第一帧的第一个数据开启定时器3做时间自动成帧处理
           Usart2_Control_Data.rx_start = 1;
           Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
       }else {
            if (Auto_Frame_Time2 <=0){   //时间超时自动成帧
               Usart2_Control_Data.rx_aframe = 1; 
               Usart2_Control_Data.rx_start = 0;
               Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index;
               Usart2_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
               return ;   //直接返回不接受数据              
            }                    
        }
       	Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_index] = rxdata;
        Usart2_Control_Data.rx_index++;
        if (Usart2_Control_Data.rx_index > (RxBufMax - 1)){
            Usart2_Control_Data.rx_index = (RxBufMax - 1);
            Usart2_Control_Data.rx_aframe = 1;  //接收数据长度自动成帧
            Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index+1;
            Usart2_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
            Usart2_Control_Data.rx_start = 0;
            Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        }   
    }else{  //接收到一帧数据后必须处理完才可以继续接受下一帧数据
       Usart2_Control_Data.rx_start = 0;
       Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        return;
    }           
}
//=============================================================================
//函数名称:Execute_Host_Comm
//功能概要:执行上位机发出的命令
//参数说明:无
//函数返回:无
//注意    :无
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
        }//把数据复制给主机通讯结构体
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
					break;
		case 0x02: 	GLED_ON;
					Gled_Light_Static = 1;
					Gled_Light_Time = Host_Control.control.gled_num * 200;	//5ms*200=1S
					Host_Control.control.gled_state = 0;
					break;
	    case 0x03: 	Gled_Num = Host_Control.control.gled_num;
					Host_Control.control.gled_state = 0;
					break;
		default : break;
		}
		switch(Host_Control.control.rled_state){
		case 0x00: 
					break;
		case 0x01: 	RLED_OFF;
					Host_Control.control.rled_state = 0;
					break;
		case 0x02: 	RLED_ON;
					Rled_Light_Static = 1;
					Rled_Light_Time = Host_Control.control.rled_num * 200;	//5ms*200=1S
					Host_Control.control.rled_state = 0;
					break;
	    case 0x03: 	Rled_Num = Host_Control.control.rled_num;
					Host_Control.control.rled_state = 0;
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
//函数名称:response_reader
//功能概要:响应扫描头数据包程序
//参数说明:无
//函数返回:返回1代表数据校验正确，返回0代表数据校验出错
//注意    :给扫描头的数据，使用CRC14校验并且只校验数据位，不校验数据头
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
		/**CRC校验OK，响应READER信号 'Y'**/
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
		/**启动发送**/
//			Beep_Num = 5;
//			Gled_Num = 5;
//			Rled_Num = 5;
//			Host_Answer.answer_state = 1;
//			Host_Answer.answer_timeout = ANSWER_SCANTIME;
		Host_Answer.answer_state = 1; //扫描头接收到正确数据后就把状态转换
		Host_Answer.start_timeout =2; //发送开始算上位机接收超时时间
		Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
		Host_Answer.answer_timeout = ANSWER_SCANTIME;
		Usart2_Control_Data.tx_index = 0;
		USART_SendData(USART2,Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index++]);
		return 1;
	}else{
		 /**CRC校验NOK，响应READER信号 'R'**/
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
		/**启动发送**/
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
		Host_Answer.answer_state = 1; //扫描头接收到正确数据后就把状态转换
		Host_Answer.start_timeout =2; //发送开始算上位机接收超时时间
		Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
		Host_Answer.answer_timeout = ANSWER_SCANTIME;
		return 1;
   	}
}
#endif
//=============================================================================
//函数名称:Copy_Scanner_Data
//功能概要:将扫描抢得到的数据复制到串口一
//参数说明:无
//函数返回:无
//注意    :无
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
//函数名称:Dispose_Data_For_Host
//功能概要:将处理后的扫描抢数据启动发送到上位机
//参数说明:无
//函数返回:无
//注意    :数据正确才上传给上位机
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
//函数名称:Respond_Host_Comm
//功能概要:响应上位机的发出的数据命令，数据已经从串口一接收完整
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void Respond_Host_Comm(void)
{
//    u8 i=0;
    if (1 == Usart1_Control_Data.rx_aframe){    
        if(Execute_Host_Comm()){
//	        /*测试程序，把电脑发过来的数据回发回去*/
//	        for(i = 0;i < Usart1_Control_Data.rx_count;i++){
//	            Usart1_Control_Data.txbuf[i] = Usart1_Control_Data.rxbuf[i];
//	        }
//	        Usart1_Control_Data.tx_count =Usart1_Control_Data.rx_count;
//	        Usart1_Control_Data.tx_index = 0;
//	        USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
//	        /*测试程序，把电脑发过来的数据回发回去*/
	        Usart1_Control_Data.rx_count = 0;
	        Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1;
			if(Beep_Num){
				TIM_Cmd(TIM4, ENABLE );
			}
			while(Beep_Num){
			   	Host_Answer.start_timeout =0; //蜂鸣器在响，证明不会超时,避免蜂鸣器响太久而错误执行超时
					Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
			}  //必须等到响应完，才可以继续接受数据
	    Scanner_Trigger_High;
			TIM_Cmd(TIM4, DISABLE );    //关闭定时器4
			delay_ms(100);
			Host_Answer.start_timeout = 1;
			Host_Answer.answer_timeout = ANSWER_SCANTIME;
		}
	    Usart1_Control_Data.rx_aframe = 0;
    }else{
        return;
    }
}

