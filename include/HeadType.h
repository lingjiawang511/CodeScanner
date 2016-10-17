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
/*******************修改扫描头只需要修改这三个参数*****************************/

//响应超时时间，即上位机确认二维码正确后扫描枪一直扫描直到得到下一次数据的最长时间
#define ANSWER_SCANTIME	 	 4000	  //4000*5ms，扫到正确的二维码，扫描头一直持续扫描的时间
//无响应超时时间，即当下位机发送数据给上位机，而上位机没有响应的最长时间
#define NANSWER_SCANTIME	 400	  //1000*5ms
//默认扫描周期中的扫描时间，扫描Ns,暂停N/2s，总的周期是3N/2s
#define DEFAULT_SCANTIME	 600

/*******************修改扫描头只需要修改这三个参数*****************************/

/******************修改蜂鸣器只需要修改这两个参数*****************************/

//蜂鸣器响声响之后停顿的时间
#define BEEP_PAUSE_TIME	60					//60*5ms=300ms
//蜂鸣器响声时间，就是响多久
#define BEEP_LOUD_TIME	1000				//1000*0.2ms =200ms
/******************修改蜂鸣器只需要修改这两个参数*****************************/
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
    u16 tx_index;        //发送当前数据的索引
    u16 rx_index;        //接收到当前数据的索引
    u16 tx_count;        //发送数据总数
    u16 rx_count;        //接收数据总数
    u8 rx_start;
    u8 rx_aframe;       //已经接收到一帧数据
    u8 txbuf[RxBufMax]; //发送数据缓存区
    u8 rxbuf[RxBufMax]; //接收数据缓存区
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


