			#include<reg52.h>
			#define OSC 11059200
			#define baudrate 9600
			#define uchar unsigned char
			uchar c,flag;
			void init()
			 {
			   TMOD=0X20;
			   SCON=0X50;	//工作方式2，REN=1
			   PCON=0X80;  //倍频
			   TL1=256-(OSC/12/16/baudrate);
			   TH1=256-(OSC/12/16/baudrate);
			   TR1=1;
			   EA=1;
			   ES=1;
			   RI=0;
			   TI=0;
			  }
			 void main(void)
			  {
			   init();
			   while(1)
			   {
			     if(flag==1)
				  {
				   ES=0;
				   flag=0;
				   SBUF=c;
				  	while(!TI);
					TI=0;
					ES=1;
				  
				  }
			   
			   }
			  }
			  void uart() interrupt 4
			  {
				   while(RI==0);
				   flag=1;
				   RI=0;
				   c=SBUF;	 //单片机取出计算机发送的值
				   
		        }