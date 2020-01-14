
/*************************************

In this system, gps is coordinating with atmega16 MCU. 
you must directly connect GPS TTL Pin (Before MAX232) pin to directly microcontroller's RX pin.
then it will provides coordinates using this program.
**********************************************/


#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>



volatile char time[10];
volatile char latitude[9];
volatile char ns;				//north south
volatile char longitude[10];
volatile char ew;				//east west
volatile char packet_start;
volatile char packet[80];

volatile char i;



void delay(uint16_t count);
char is_GPGGA(const char *str);



/*
void init_interrupt()
{
	MCUCR|=(3<<ISC00);
	GICR|=(1<<INT0);
}
*/

void main()
{	
	int abc=0;
	char flag1, flag2;	
	
	DDRB &=~(1<<0);
	PORTB|=(1<<0);
	USARTInit(71);
	delay(10);
	sei(); //Interrupt service Enable
	
	while(1)
	{

		flag1=PINB&(1<<0);

		if((flag1==0) && (flag2!=0))
		{
		_delay_ms(5);

		if(!(PINB&(1<<0)))
		{

			_delay_ms(1000);

			
			write("lat=");
			_delay_ms(500);

			for(int temp=0;temp<=8;temp++)
			{
			 	USARTWriteChar(latitude[temp]);
			}
			
			USARTWriteChar(ns);
			_delay_ms(500);
			
			write(" long=");
			for(int temp=0;temp<=9;temp++)
				USARTWriteChar(longitude[temp]);
			}USARTWriteChar(ew);

			write("\r\n");
			_delay_ms(5000);

			LCDClear();
			USARTWriteChar(0x1A);
			LCDWriteStringXY(0,0,"msg send");
			_delay_ms(2000);
			
		//	delay(10);
			LCDClear();

			}
		}
		flag2=flag1;

	}
		
}



void delay(uint16_t count)
{
	uint16_t i;
	for(i=0;i<count;i++)
	_delay_loop_2(0);
}

char is_GPGGA(const char *str)
{
	char i;
	/*char type[5];
	type[0]=str[1];
	type[1]=str[2];
	type[2]=str[3];
	type[3]=str[4];
	type[4]=str[5];*/
	
	if(str[1]=='G')
	{
		
		
		if(str[2]=='P')
		{
			
			if(str[3]=='G')
			{
				
				if(str[4]=='G')
				{
					
					if(str[5]=='A')
					return 1;
					else return 0;
				}
				else return 0;
			}
			else return 0;
		}
		else return 0;
	}
	else return 0;
}
	


	

	

ISR(USART_RXC_vect)
{
	char data;
	
	data=USARTReadChar();
	
	if(data=='$')
	{
		packet_start=1;
		i=0;
	}
	if(packet_start==1)
	{
		if(data!='*')
		{
			packet[i++]=data;
			//i+=1;
			//USARTWriteChar(packet[i-1]);
			
			
		}
		else
		{
			packet_start=0;
			if(is_GPGGA(packet))
			{
				
				
				latitude[0]=packet[18];
				latitude[1]=packet[19];
				latitude[2]=packet[20];
				latitude[3]=packet[21];
				latitude[4]=packet[22];
				latitude[5]=packet[23];
				latitude[6]=packet[24];
				latitude[7]=packet[25];
				latitude[8]=packet[26];
				
				

				ns=packet[28];
				
				longitude[0]=packet[30];
				longitude[1]=packet[31];
				longitude[2]=packet[32];
				longitude[3]=packet[33];
				longitude[4]=packet[34];
				longitude[5]=packet[35];
				longitude[6]=packet[36];
				longitude[7]=packet[37];
				longitude[8]=packet[38];
				longitude[9]=packet[39];


				ew=packet[41];
				_delay_ms(5);
				
			}
			else 
			{
			continue;

			}
		}
	}

}


ISR(USART_TXC_vect)
{
 cli();
}


	


