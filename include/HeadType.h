#ifndef __HEADTYPE_H_
#define __HEADTYPE_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>	
#include "led.h"
#include "delay.h"
#include "beep.h"
#include "codescanner.h"

/*************Typedef datatype start*******************/
typedef char int8;
typedef volatile char vint8;
typedef unsigned char uint8;
typedef volatile unsigned char vuint8;
typedef int int16;
typedef unsigned short uint16;
typedef long int32;
typedef unsigned long uint32;
typedef uint8	uchar,u8,BYTE;		/**< 8-bit value */
typedef uint8 	SOCKET;
typedef uint16	ushort,u16,WORD;	/**< 16-bit value */
typedef uint16	uint;		/**< 16-bit value */
typedef uint32	ulong;		/**< 32-bit value */
/*************Typedef datatype end*******************/

/*************define type start*******************/
#define false      (0)
#define true       (!false)

#ifndef NULL
#define NULL  ((void *)(0))
#endif

#define	SCAN_CR8000 1

#define SCAN_1400G	!SCAN_CR8000
#define RxBufMax 512
#define TxBufMax 512
/*******************�޸�ɨ��ͷֻ��Ҫ�޸�����������*****************************/

//��Ӧ��ʱʱ�䣬����λ��ȷ�϶�ά����ȷ��ɨ��ǹһֱɨ��ֱ���õ���һ�����ݵ��ʱ��
#define ANSWER_SCANTIME	 	 4000	  //4000*5ms��ɨ����ȷ�Ķ�ά�룬ɨ��ͷһֱ����ɨ���ʱ��
//����Ӧ��ʱʱ�䣬������λ���������ݸ���λ��������λ��û����Ӧ���ʱ��
#define NANSWER_SCANTIME	 400	  //1000*5ms
//Ĭ��ɨ�������е�ɨ��ʱ�䣬ɨ��Ns,��ͣN/2s���ܵ�������3N/2s
#define DEFAULT_SCANTIME	 600

/*******************�޸�ɨ��ͷֻ��Ҫ�޸�����������*****************************/

/******************�޸ķ�����ֻ��Ҫ�޸�����������*****************************/

//������������֮��ͣ�ٵ�ʱ��
#define BEEP_PAUSE_TIME	60					//60*5ms=300ms
//����������ʱ�䣬��������
#define BEEP_LOUD_TIME	1000				//1000*0.2ms =200ms
/******************�޸ķ�����ֻ��Ҫ�޸�����������*****************************/
/*************define type end*******************/

/*************union type start*******************/
typedef union{
	long l;
	unsigned long ul;
	int  i;
	unsigned int ui;
	short si[2];
	char  c[4];
	unsigned char uc[4];	
}UL;

typedef union{
	short i;
	unsigned short ui;
	char c[2];
	unsigned char uc[2];
}UI;

/*************union type end*******************/

/*************enum type start*******************/
enum
{
	False=0,True
};
/*************enum type end*******************/

/*************struct type start*******************/

typedef struct{
    u16 tx_index;        //���͵�ǰ���ݵ�����
    u16 rx_index;        //���յ���ǰ���ݵ�����
    u16 tx_count;        //������������
    u16 rx_count;        //������������
    u8 rx_start;
    u8 rx_aframe;       //�Ѿ����յ�һ֡����
    u8 txbuf[RxBufMax]; //�������ݻ�����
    u8 rxbuf[RxBufMax]; //�������ݻ�����
}Usart_Type;

typedef struct{
	u8  frame_soh;
	u8  frame_x;
	u8  frame_r;
	u8  frame_1;
	u16 data_size;
	u8  beep_state;
	u8  beep_num;
	u8  gled_state;
	u8  gled_num;
	u8  rled_state;
	u8  rled_num;
	u8  scanner_state;
	u8  scanner_time;
	u16 crc16_ccitt; 
}Host_Communation_Type;
typedef struct{
	u8  answer_state;
	u8 start_timeout;
	u16 answer_timeout;
	u16 Nanswer_timeout;
}Host_Answer_Type;

typedef union{
	Host_Communation_Type control;
	u8	host_buf[16];	
}Host_COMM_Union_Type;

/*************struct type end*******************/

/*************extern variable start*******************/
extern char Auto_Frame_Time1;
extern char Auto_Frame_Time2;
extern Usart_Type Usart1_Control_Data;
extern Usart_Type Usart2_Control_Data;
extern Host_COMM_Union_Type Host_Control;
extern Host_Answer_Type 	 Host_Answer;
/*************extern variable end*******************/

/*************function start*******************/
u16 CRC_GetModbus16(u8 *pdata, int len);
u16 CRC_GetCCITT(u8 *pdata, int len);
/*************function end*******************/
#endif


