	   #include<reg51.h>
	   #define uchar unsigned char
	   uchar code table[]=
	   {
	   	0xc0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
        0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
	   };
	  
	   uchar c;
	   uchar count;
	   void delay(uchar z) ;
	   void init(void)
	   {
	  	  
		TMOD=0X05;
		TH0=0;
		TL0=0;
		TR0=1;
		P0=table[0];
		count=0;
		c=TL0;

	   
	   }
	  
	  void main(void)
	   { 
	    init();
		
		IT0=0;
		EX0=1;
		EA=1;
		while(1);
		 
		
		}
	    
		
		void Counter0() interrupt 1
		 {
		    
				if(c!=TL0)
		        { 
				  delay(10);
				  if(c!=TL0)
				   { c=TL0;
			         count++;
					 if(count>=10)
						  count=0;
					 P0=table[count];
				     
					}
	             }
		  }
		 void delay(uchar z)
		  {
		   uchar x,y;
		    for(x=0;x<z;x++)
		     for(y=0;y<250;y++);
		   }
		   
		 
		
	   
	   