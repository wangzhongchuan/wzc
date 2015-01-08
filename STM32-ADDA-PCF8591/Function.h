#include "stm32f10x_lib.h"


/****funtion.h函数声明部分***********/

extern u8 ack;

//时序延时函数
extern void Delay_us(u32 nTime);

//起动总线函数
extern void Start_I2c(void);

//结束总线函数  
extern void Stop_I2c(void);

//应答子函数
extern void Ack_I2c(u8 a);

//字节数据发送函数
extern void  SendByte(unsigned char  c);

//有子地址发送多字节数据函数               
extern u8 ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no) ;

//无子地址发送多字节数据函数   
extern u8 ISendStrExt(unsigned char sla,unsigned char *s,unsigned char no);

//无子地址读字节数据函数               
extern unsigned char RcvByte(void);

//有子地址读取多字节数据函数               
extern u8 IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

//无子地址读取多字节数据函数               
extern u8 IRcvStrExt(unsigned char sla,unsigned char *s,unsigned char no);





