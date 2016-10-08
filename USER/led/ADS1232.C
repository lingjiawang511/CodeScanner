#include "ADS1232.h" 
#include "delay.h"


unsigned char FlagADC;

void ADS1232_GPIO_Init(void)
{
    int i=0;
  //  uint32_t ad_value=0;
   	//定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//使能GPIOC的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
   // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	///设置引脚模式为推免输出模式			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//设置引脚速度为50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//调用库函数，初始化GPIO
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    #ifndef DACTEST 
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    #endif

   
    //选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	///设置引脚模式为推免输出模式			 
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;  		 
	//设置引脚速度为50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//调用库函数，初始化GPIO
	GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    Reset_AD_SCK ;  
    Reset_AD_DOWN ;
   // Set_AD_SPEED ;
    Reset_AD_SPEED ;
    delay_us(20);
    Set_AD_DOWN ; 
    Set_AD_SCK ;  
    for(i=0;i<25;i++)
        {
            Set_AD_SCK ;  
            delay_us(2);
            Reset_AD_SCK ; 
            delay_us(2);
         }
}

uint32_t AD_read(uint8_t Adjust)
 {
    u8 i=0;
    uint32_t ad_value=0;
     
    while(Read_AD_DOUT==Bit_SET);
  //  delay_us(20);
  //  FlagADC=1;
    for(i=0;i<24;i++)
        {
            ad_value=ad_value<<1;
            Set_AD_SCK ;  
            delay_us(1);
            if(Read_AD_DOUT==1)
            {
               // ad_value|=0x00000001;  
                ad_value+=1; 
            }
            else 
               ad_value+=0;   
            Reset_AD_SCK ; 
            delay_us(2);
         }
    Set_AD_SCK ;
    delay_us(2);
    Reset_AD_SCK ; 
    if(Adjust) 
    {
        Set_AD_SCK ;
        delay_us(2);
        Reset_AD_SCK ;   
    }
 //  delay_ms(20);

   return ad_value;
 }

//以下滤波
//以下滤波
static u8 BufferLen;
static  u8 FilterIndex=0;
static  long  sum=0;
static  long  FilterBuffer[32]={0};
long FilterOut=0;
UL ADS1232_Valu;
void InitFilter(long dat)
{
	u8 i;
	BufferLen=(1<<param.av3.l);
	for ( i = 0 ; i <BufferLen; i++ )
	{
	    FilterBuffer[i]=dat;
	}
	sum=(dat<<param.av3.l);
}
long abs(long a,long b)
{
	if(a>b)return (a-b);
	return (b-a);
}

void Filter(long Newdat)
{
	static u8 Count=0;
	if(abs(Newdat,FilterOut)>param.pv3.l)
	{
		Count++;
		if(Count>3)
		{
			InitFilter(Newdat);
			FilterOut=Newdat;
			return;//直通
		}
	}
	else
	{
		Count=0;
	}
	sum=sum+Newdat-FilterBuffer[FilterIndex];//滑动
	FilterBuffer[FilterIndex]=Newdat;
	if(++FilterIndex>=BufferLen)
	{
		FilterIndex=0;
	}
	FilterOut=(sum>>param.av3.l);
}
