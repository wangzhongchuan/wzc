/***************18B20�¶���ʾ����*****************/

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


/*******************UART()��ʼ������*******************/

void UART()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XF3;				//��������ʼֵ���ã�ע�Ⲩ������4800��
	TL1=0XF3;
	ES=1;						//�򿪽����ж�
    EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
}


void main()
{
 	led=0;
	buzzer=0;
	UART();
	LcdInit();			 //��ʼ��LCD1602
	LcdWriteCom(0x88);	 //д��ַ 80��ʾ��ʼ��ַ
	LcdWriteData('C');

	while(1)
	{
		LcdDisplay(Ds18b20ReadTemp());
		  if(Ds18b20ReadTemp()>350)
			{
			  led=1;
			  for(i=0;i<10;i--)		   //������Դ������	 ����Ҫ��������һ������������ſ��ԣ������forѭ��
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

/***************** LcdDisplay()LCD��ʾ��ȡ�����¶�*******************/

void LcdDisplay(int temp) 	 //lcd��ʾ
{
    
  	unsigned char i,  datas[] = {0, 0, 0, 0, 0}; //��������
	float tp;  
	if(temp< 0)				    //���¶�ֵΪ����
  	{
	  	LcdWriteCom(0x80);		//д��ַ 80��ʾ��ʼ��ַ
		SBUF='-';               //�����յ������ݷ��뵽���ͼĴ���
		while(!TI);			         //�ȴ������������
		TI=0;						 //���������ɱ�־λ
	    LcdWriteData('-');  	     //��ʾ��
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
 
  	}
 	else
  	{			
	  	
		tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
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
	   while(!TI);	        //�ȴ������������
	   TI=0;
	  
	  }
	LcdWriteCom(0x80+0x40);		//д��ַ ��ʾ��ʼ��ַ
    LcdWriteData('+'); 		//��ʾ��
    SBUF='+';//�����յ������ݷ��뵽���ͼĴ���
	while(!TI);			         //�ȴ������������
	TI=0;						 //���������ɱ�־λ
	LcdWriteCom(0x80+0x40+0x02);		    //д��ַ ��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[0]); //��λ 
	SBUF = '0'+datas[0];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;
	
	LcdWriteCom(0x80+0x40+0x03);		    //д��ַ ��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[1]); //ʮλ
	SBUF = '0'+datas[1];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;

	LcdWriteCom(0x80+0x40+0x04);		    //д��ַ ��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[2]); //��λ 
	SBUF = '0'+datas[2];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;

	LcdWriteCom(0x80+0x40+0x05);		//д��ַ ��ʾ��ʼ��ַ
	LcdWriteData('.'); 		//��ʾ ��.��
	SBUF = '.';             //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			//�ȴ������������
	TI = 0;

	LcdWriteCom(0x80+0x40+0x06);		    //д��ַ ��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[3]); //��ʾС����  
	SBUF = '0'+datas[3];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;

	LcdWriteCom(0x80+0x40+0x07);		    //д��ַ ��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[4]); //��ʾС���� 
	SBUF = '0'+datas[4];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;
	LcdWriteCom(0x80+0x40+0x09);
    for(i=0;i<1;i++)
	   {
	    LcdWriteData(table[i]);	
		SBUF=table[i];       //�����յ������ݷ��뵽���ͼĴ���
		while(!TI);	        //�ȴ������������
		TI=0;
	   }
	 

}




	

	 
