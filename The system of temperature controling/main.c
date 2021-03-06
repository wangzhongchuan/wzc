/***************18B20温度显示试验*****************/

#include<reg51.h>
#include"lcd.h"
#include"temp.h"

uchar table[]={'C'};
uchar table1[12]={"temperature:"};
void LcdDisplay(int);
sbit buzzer=P1^0;
sbit led=P1^6;
int i;
 void delay(unsigned int x)
{
    char y;
    for(x; x > 0; x--)
        for(y = 200; y > 0; y--);
}


/*******************UART()初始化串口*******************/

void UART()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XF3;				//计数器初始值设置，注意波特率是4800的
	TL1=0XF3;
	ES=1;						//打开接收中断
    EA=1;						//打开总中断
	TR1=1;					//打开计数器
}


void main()
{
 	led=0;
	buzzer=0;
	UART();
	LcdInit();			 //初始化LCD1602
	LcdWriteCom(0x88);	 //写地址 80表示初始地址
	LcdWriteData('C');

	while(1)
	{
		LcdDisplay(Ds18b20ReadTemp());
		  if(Ds18b20ReadTemp()>350)
			{
			  led=1;
			  for(i=0;i<10;i--)		   //驱动无源蜂鸣器	 ，需要给蜂鸣器一个持续的脉冲才可以，即这个for循环
			   {
			   		 buzzer= 0;
					 delay(1);
					 buzzer= 1;
					 delay(1);	
				}	
 
			} 
       else {
		      led=0;   	 
		     } 

	}

}

/***************** LcdDisplay()LCD显示读取到的温度*******************/

void LcdDisplay(int temp) 	 //lcd显示
{
    
  	unsigned char i,  datas[] = {0, 0, 0, 0, 0}; //定义数组
	float tp;  
	if(temp< 0)				    //当温度值为负数
  	{
	  	LcdWriteCom(0x80);		//写地址 80表示初始地址
		SBUF='-';               //将接收到的数据放入到发送寄存器
		while(!TI);			         //等待发送数据完成
		TI=0;						 //清除发送完成标志位
	    LcdWriteData('-');  	     //显示负
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由�0.5，还是在小数点后面。
 
  	}
 	else
  	{			
	  	
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;
	LcdWriteCom(0x80);
	 for(i=0;i<12;i++)
	  {
	   LcdWriteData(table1[i]);
	   SBUF=table1[i];
	   while(!TI);	        //等待发送数据完成
	   TI=0;
	  
	  }
	LcdWriteCom(0x80+0x40);		//写地址 表示初始地址
    LcdWriteData('+'); 		//显示正
    SBUF='+';//将接收到的数据放入到发送寄存器
	while(!TI);			         //等待发送数据完成
	TI=0;						 //清除发送完成标志位
	LcdWriteCom(0x80+0x40+0x02);		    //写地址 表示初始地址
	LcdWriteData('0'+datas[0]); //百位 
	SBUF = '0'+datas[0];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;
	
	LcdWriteCom(0x80+0x40+0x03);		    //写地址 表示初始地址
	LcdWriteData('0'+datas[1]); //十位
	SBUF = '0'+datas[1];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;

	LcdWriteCom(0x80+0x40+0x04);		    //写地址 表示初始地址
	LcdWriteData('0'+datas[2]); //个位 
	SBUF = '0'+datas[2];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;

	LcdWriteCom(0x80+0x40+0x05);		//写地址 表示初始地址
	LcdWriteData('.'); 		//显示 ‘.’
	SBUF = '.';             //将接收到的数据放入到发送寄存器
	while (!TI);			//等待发送数据完成
	TI = 0;

	LcdWriteCom(0x80+0x40+0x06);		    //写地址 表示初始地址
	LcdWriteData('0'+datas[3]); //显示小数点  
	SBUF = '0'+datas[3];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;

	LcdWriteCom(0x80+0x40+0x07);		    //写地址 表示初始地址
	LcdWriteData('0'+datas[4]); //显示小数点 
	SBUF = '0'+datas[4];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;
	LcdWriteCom(0x80+0x40+0x09);
    for(i=0;i<1;i++)
	   {
	    LcdWriteData(table[i]);	
		SBUF=table[i];       //将接收到的数据放入到发送寄存器
		while(!TI);	        //等待发送数据完成
		TI=0;
	   }
	 

}




	

	 
