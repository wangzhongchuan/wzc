/
* 
*                      RCC配置实验
* 

*  #define SYSCLK_FREQ_20MHz	   //20MHZ
			 #define SYSCLK_FREQ_36MHz	  //36MH工作
			 #define SYSCLK_FREQ_48MHz	  //48MH工作 通过宏定义改变系统时钟频率
			 #define SYSCLK_FREQ_72MHz	  //72MH
*******************************************************************************/


#include "stm32f10x_lib.h"


/******************************** 宏定义 ------------------------------------*/
//#define SYSCLK_HSE
#define SYSCLK_FREQ_20MHz
//#define SYSCLK_FREQ_36MHz	  //36MH工作
//#define SYSCLK_FREQ_48MHz	  //48MH工作 通过宏定义改变系统时钟频率
//#define SYSCLK_FREQ_72MHz	  //工作72MH 

#define DELAY_COUNT    0xFFFFF

/******************************** 变量定义 ------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
RCC_ClocksTypeDef  RCC_ClockFreq;
ErrorStatus  HSEStartUpStatus;

/*********************************声明函数 -----------------------------------------------*/
void SetSysClock(void);
void SetSysClockToHSE(void);
void SetSysClockTo20(void);
void SetSysClockTo36(void);
void SetSysClockTo48(void);
void SetSysClockTo72(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);


/*******************************************************************************
* 
*            主函数
* 
*******************************************************************************/
int main(void)
{
#ifdef DEBUG
  debug();
#endif

 
  SetSysClock();   //设置系统时钟  HCLK PCLK2 PCLK1 

  /* This function fills the RCC_ClockFreq structure with the current
     frequencies of different on chip clocks (for debug purpose) */
  RCC_GetClocksFreq(&RCC_ClockFreq);

  /* Enable Clock Security System(CSS): this will generate an NMI exception
     when HSE clock fails */
  RCC_ClockSecuritySystemCmd(ENABLE);
     
  /* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  //开启GPIOA 和 GPIOB 模块时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			  //推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_SetBits(GPIOB, GPIO_Pin_7 | GPIO_Pin_9);

  /* Output HSE clock on MCO pin */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //开漏输出
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

  RCC_MCOConfig(RCC_MCO_HSE);

  /* Toggle leds connected to GPIO_LED Pin 6, Pin 7, Pin 8 and Pin 9 pins */     
  while (1)
  {
    GPIO_Write(GPIOB, (u16)~GPIO_ReadOutputData(GPIOB));

    /* Insert a delay */
    Delay(DELAY_COUNT);
  }
}

/*******************************************************************************
* Function Name  : SetSysClock
* Description    : Configures the System clock frequency, HCLK, PCLK2 and PCLK1
*                  prescalers.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetSysClock(void)
{    
#if defined SYSCLK_HSE
  SetSysClockToHSE();
#elif defined SYSCLK_FREQ_20MHz
  SetSysClockTo20();
#elif defined SYSCLK_FREQ_36MHz
  SetSysClockTo36();
#elif defined SYSCLK_FREQ_48MHz
  SetSysClockTo48();
#elif defined SYSCLK_FREQ_72MHz
  SetSysClockTo72();
#endif
 
 /* If none of the define above is enabled, the HSI is used as System clock
    source (default after reset) */ 
}

/*******************************************************************************
* Function Name  : SetSysClockToHSE
* Description    : Selects HSE as System clock source and configure HCLK, PCLK2
*                  and PCLK1 prescalers. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetSysClockToHSE(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 0 wait state */
    FLASH_SetLatency(FLASH_Latency_0);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK */
    RCC_PCLK1Config(RCC_HCLK_Div1);

    /* Select HSE as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x04)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }  
}

/*******************************************************************************
* Function Name  : SetSysClockTo20
* Description    : Sets System clock frequency to 20MHz and configure HCLK, PCLK2 
*                  and PCLK1 prescalers. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetSysClockTo20(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();										//将RRC寄存器设为缺省值

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);					   //开启外部高速晶振

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();		  //等待HSE起震

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	   //flash模式选择

    /* Flash 0 wait state */
    FLASH_SetLatency(FLASH_Latency_0);						//0延时周期
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 						//AHB时钟为原系统时钟，不分频	AHB总线
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 						 //高速AHB时钟为系统时钟   APB2总线

    /* PCLK1 = HCLK */
    RCC_PCLK1Config(RCC_HCLK_Div1);							 //低速AHB时钟为系统时钟  APB1总线

    /* PLLCLK = (8MHz / 2) * 5 = 20 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_5);	//	PLL时钟		  为 PLLCLK = (8MHz / 2) * 5 = 20 MHz

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);									 //使能PLL时钟

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)			//检测RCC标志位是否设置
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);						//选择PLL时钟作为系统时钟

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }
}

/*******************************************************************************
* Function Name  : SetSysClockTo36
* Description    : Sets System clock frequency to 36MHz and configure HCLK, PCLK2 
*                  and PCLK1 prescalers. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetSysClockTo36(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 1 wait state */
    FLASH_SetLatency(FLASH_Latency_1);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK */
    RCC_PCLK1Config(RCC_HCLK_Div1);

    /* PLLCLK = (8MHz / 2) * 9 = 36 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);	   //这里决定时钟大小，其他与20M一致

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }
}

/*******************************************************************************
* Function Name  : SetSysClockTo48
* Description    : Sets System clock frequency to 48MHz and configure HCLK, PCLK2 
*                  and PCLK1 prescalers. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetSysClockTo48(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 1 wait state */
    FLASH_SetLatency(FLASH_Latency_1);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 6 = 48 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }
}

/*******************************************************************************
* Function Name  : SetSysClockTo72
* Description    : Sets System clock frequency to 72MHz and configure HCLK, PCLK2 
*                  and PCLK1 prescalers. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetSysClockTo72(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM  												 //设置向量表位置和偏移
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  /* Enable and configure RCC global IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
{
  for(; nCount!= 0;nCount--);
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

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
