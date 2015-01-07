/********************普中科技  www.prechin.com ************ ********************
* 
*                          SysTick定时器实验
*  
*    实验目的：	熟悉了解SysTick配置和滴答定时设定
* 
*    接线方法： 参考“接线演示”图片
* 
*    实验现象：	D1和D2交替点亮
* 
*******************************************************************************/

#include "stm32f10x_lib.h"
 
/******************************** 变量定义 ------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
static vu32 TimingDelay;
ErrorStatus HSEStartUpStatus;
void TimingDelay_Decrement(void);

/*********************************声明函数 -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nTime);


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

  RCC_Configuration();   //系统时钟配置 
   
  NVIC_Configuration();	 //NVIC配置

   //系统默认SysTick是8分频（HCLK/8）当前系统时钟72M的话 72/8 = 9MHZ
  SysTick_SetReload(9000); //如果设置9000时，计数到9000时，产生1MS中断	   定时器装初值

  //开启SysTick中断
  SysTick_ITConfig(ENABLE);												 //启动
 
  //开启GPIOB端口
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  GPIO_Write(GPIOB, GPIO_Pin_6 | GPIO_Pin_8);

  while (1)
  {

    GPIO_Write(GPIOB, (u16)~GPIO_ReadOutputData(GPIOB));
    Delay(500);	  //延时500MS
     GPIO_Write(GPIOB, (u16)~GPIO_ReadOutputData(GPIOB));
    Delay(500);	 //延时500MS  
  }
}


/*******************************************************************************
* 
*            RCC配置
* 
*******************************************************************************/
void RCC_Configuration(void)
{   
  //复位RCC外部设备寄存器到默认值
  RCC_DeInit();  

  //打开外部高速晶振
  RCC_HSEConfig(RCC_HSE_ON); 

 //等待外部高速时钟准备好
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); 

  //外部高速时钟已经准别好
  if(HSEStartUpStatus == SUCCESS)  
  {
    
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

   
    FLASH_SetLatency(FLASH_Latency_2);
  
   //配置AHB(HCLK)时钟=SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
  
    //配置APB2(PCLK2)钟=AHB时钟
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    //配置APB1(PCLK1)钟=AHB 1/2时钟
    RCC_PCLK1Config(RCC_HCLK_Div2);  

    //配置ADC时钟=PCLK2 1/4
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
  
    //配置PLL时钟 == 外部高速晶体时钟*9
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    
	//配置ADC时钟= PCLK2/4
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

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

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
}

/*******************************************************************************
* 
*           对系统滴答做延时处理
* 
*******************************************************************************/
void Delay(u32 nTime)
{
  //开启SysTick计数器
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  TimingDelay = nTime;

  while(TimingDelay != 0);	   //死等待


  //关闭系统滴答
  SysTick_CounterCmd(SysTick_Counter_Disable);
  //清除SysTick 计数器
  SysTick_CounterCmd(SysTick_Counter_Clear);
}


/*******************************************************************************
*
*          SysTick中断处理函数
* 
*******************************************************************************/
void SysTick_Handler(void)
{
   if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
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
