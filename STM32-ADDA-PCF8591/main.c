/**********************************************************************************
    ************* STM32 模拟I2C-DA-AD试验*******************           
		                   	      
***********************************************************************************/

#include "stm32f10x_lib.h"	 
#include"Function.h"

 /********************************硬件接口定义*********************************/
#define    LS138A  GPIO_Pin_5 //=P2^2;       //138译码器管脚定义
#define    LS138B  GPIO_Pin_4 //=P2^3;
#define    LS138C  GPIO_Pin_3 //=P2^4; 


/********************************宏定义*********************************/
#define    LS138a(x)  x ? GPIO_SetBits(GPIOB, LS138A): GPIO_ResetBits(GPIOB, LS138A)
#define    LS138b(x)  x ? GPIO_SetBits(GPIOB, LS138B): GPIO_ResetBits(GPIOB, LS138B)
#define    LS138c(x)  x ? GPIO_SetBits(GPIOB, LS138C): GPIO_ResetBits(GPIOB, LS138C)

#define  PCF8591 0x90    //PCF8591 地址



/******************************** 变量定义 ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;     //GPIO
ErrorStatus HSEStartUpStatus;

unsigned long D[16], LedOut[8];
unsigned char AD_CHANNEL;
static vu32 TimingDelay;

//此表为 LED 的字模, 共阴数码管 0-9  - 
unsigned char  Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 

/*********************************声明函数 -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void SysTick_Configuration(void);
void Delay_us(u32 nTime);
u8 DACconversion(unsigned char sla,unsigned char c,  unsigned char Val);
u8 ISendByte(unsigned char sla,unsigned char c);
u8 IRcvByte(unsigned char sla);

 main()
 { 	unsigned char i;
	
	#ifdef DEBUG
	debug();  //在线调试使用
	#endif
	
	RCC_Configuration();      //系统时钟配置函数 
	
	NVIC_Configuration();     //NVIC配置函数 
	
    SysTick_Configuration();   //Systick配置函数
	
	
	//启动GPIO模块时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB1Periph_TIM2 | RCC_APB2Periph_AFIO, ENABLE);
	//把调试设置普通IO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  
	                        
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //所有GPIO为同一类型端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA端口
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB端口

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11   ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOA端口

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //开漏输出	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOA端口
	
	GPIO_Write(GPIOA,0xffff);  //将GPIOA 16个端口全部置为高电平
	GPIO_Write(GPIOB,0xffff);  //将GPIOB 16个端口全部置为高电

   while(1)
   {
	   /********以下AD-DA处理*************/  
	   switch(AD_CHANNEL)
	   {
	     case 0: ISendByte(PCF8591,0x41);
	             D[0]=IRcvByte(PCF8591)*2;  //ADC0 模数转换1  放大2倍显示
				 break;  
	 
		 case 1: ISendByte(PCF8591,0x42);
	             D[1]=IRcvByte(PCF8591)*2;  //ADC1  模数转换2
				 break;  
	
		 case 2: ISendByte(PCF8591,0x43);
	             D[2]=IRcvByte(PCF8591)*2;  //ADC2	模数转换3
				 break;  
	
		 case 3: ISendByte(PCF8591,0x40);
	             D[3]=IRcvByte(PCF8591)*2;  //ADC3   模数转换4
				 break;  
	
		 case 4: DACconversion(PCF8591,0x40, D[4]/4); //DAC	  数模转换
		         break;
	   }
	
		      // D[4]=400;  //数字--->>模拟输出
			  D[4]=D[0]*4;  //   把模拟输入 采样的信号 通过数模转换输出
	   if(++AD_CHANNEL>4) AD_CHANNEL=0;
	
	 /********以下将AD的值送到LED数码管显示*************/
    
	  LedOut[0]=Disp_Tab[D[0]%10000/1000];
      LedOut[1]=Disp_Tab[D[0]%1000/100];
      LedOut[2]=Disp_Tab[D[0]%100/10]|0x80;
      LedOut[3]=Disp_Tab[D[0]%10];
	  
	  LedOut[4]=Disp_Tab[D[0]%10000/1000];
      LedOut[5]=Disp_Tab[D[0]%1000/100];
      LedOut[6]=Disp_Tab[D[0]%100/10]|0x80;
      LedOut[7]=Disp_Tab[D[0]%10];  
  	  
      for(i=0; i<8; i++) 
  	  {		
     	GPIOA->BSRR = LedOut[i] & 0x00FF;   
        GPIOA->BRR = (~LedOut[i]) & 0x00FF;
	  			 	 	  	
		switch(i)					  
         {	    	  //138译码
			case 0:	  //0 0 0 
			LS138a(0);	 LS138b(0);  LS138c(0);
			break;         
            case 1:	 //1 0 0 
			LS138a(1);	 LS138b(0);  LS138c(0);
			 break;             	
            case 2:	 //0 1 0
			LS138a(0);	 LS138b(1);  LS138c(0);
			break; 
            case 3:	 //1 1 0
			LS138a(1);	 LS138b(1);  LS138c(0);
			break;
			
			case 4:	 //0 0 1
			LS138a(0);	 LS138b(0);  LS138c(1);
			break; 
			
			case 5:	 //1 0 1
			LS138a(1);	 LS138b(0);  LS138c(1);
			break; 

			case 6:	 //0 1 1
			LS138a(0);	 LS138b(1);  LS138c(1);
			break;

			case 7:	 //1 1 1
			LS138a(1);	 LS138b(1);  LS138c(1);
			break;
		 	 
         }
		Delay_us(600); 
	  }

	}
   
}



/*******************************************************************
DAC 变换, 转化函数               
*******************************************************************/
u8 DACconversion(unsigned char sla,unsigned char c,  unsigned char Val)
{
   Start_I2c();              //启动总线
   SendByte(sla);            //发送器件地址
   if(ack==0)return(0);
   SendByte(c);              //发送控制字节
   if(ack==0)return(0);
   SendByte(Val);            //发送DAC的数值  
   if(ack==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}

/*******************************************************************
ADC发送字节[命令]数据函数               
*******************************************************************/
u8 ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();              //启动总线
   SendByte(sla);            //发送器件地址
   if(ack==0)return(0);
   SendByte(c);              //发送数据
   if(ack==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}

/*******************************************************************
ADC读字节数据函数               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c;

   Start_I2c();          //启动总线
   SendByte(sla+1);      //发送器件地址
   if(ack==0)return(0);
   c=RcvByte();          //读取数据0

   Ack_I2c(1);           //发送非就答位
   Stop_I2c();           //结束总线
   return(c);
}

/******************************************************************************
*
*                         精准延时函数
*
**********************************************************************************/
void Delay_us(u32 nTime)
{
  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  TimingDelay = nTime;

  while(TimingDelay != 0);

  /* Disable SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);
  /* Clear SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
*
*                         系统Systick 中断处理函数
*
*******************************************************************************/
void SysTick_Handler(void)   
{   
     if (TimingDelay != 0x00)
  { 
    TimingDelay--;
	
  }

} 


/*******************************************************************************
*                           配置RCC
*******************************************************************************/
void RCC_Configuration(void)
{   
 //复位RCC外部设备寄存器到默认值
  RCC_DeInit();

  //打开外部高速晶振
  RCC_HSEConfig(RCC_HSE_ON);

   //等待外部高速时钟准备好
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)   //外部高速时钟已经准别好
  {								    
    //开启FLASH的预取功能
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    //FLASH延迟2个周期
    FLASH_SetLatency(FLASH_Latency_2);
 	
  //配置AHB(HCLK)时钟=SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
  
   //配置APB2(PCLK2)钟=AHB时钟
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    //配置APB1(PCLK1)钟=AHB 1/2时钟
    RCC_PCLK1Config(RCC_HCLK_Div2);

     //配置PLL时钟 == 外部高速晶体时钟*9  PLLCLK = 8MHz * 9 = 72 MHz 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    //使能PLL时钟
    RCC_PLLCmd(ENABLE);

   //等待PLL时钟就绪
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

  //配置系统时钟 = PLL时钟
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

   //检查PLL时钟是否作为系统时钟
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
}


void SysTick_Configuration(void)   
{   
  /*  设置AHB时钟为SysTick时钟*/  
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  //采用8分频 
  
  /* 设置SysTicks中断抢占优先级 3， 从优先级0*/  
  NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 3, 0);   
      
  /* 每1ms发生一次SysTick中断 在72M的时钟下*/  
  SysTick_SetReload(9);     //1微秒计数结束
  
  /* 开启SysTick中断 */  
  SysTick_ITConfig(ENABLE);   
}  


/*******************************************************************************
*                             NVIC配置函数
*******************************************************************************/
void NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  
       /* 开启定时器2 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
