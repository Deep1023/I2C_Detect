/*LPC2129*/
/*I2C_detect_driver.c*/
/*dipdmp99@gmail.com*/

#include<lpc21xx.h>
#define SI ((I2CONSET>>3)&1)
#define THRE ((U0LSR>>5)&1)
#include "header.h"

void delay_us(unsigned int i)
{
	T0PR=15-1;
	T0PC=0;
	T0TC=0;
	T0TCR=1;
	while(T0TC<i);
	T0TCR=0;
}

//=========================================================================================================

void i2c_init(void)
{
	PINSEL0|=0x50;//select pin 0.2 and 0.3 as sdl and sck line
	I2SCLL=75;//for 100kbps speed
	I2SCLH=75;//for 100kbps speed
	I2CONSET=1<<6;//enable I2c 
}

//=========================================================================================================

void i2c_detect(void)
{
	unsigned char i,j;
	
	for(i=0x0,j=0 ; i<=0xFE ; i++)
	{
		I2CONSET=1<<5 ; //generate start condition
		while(SI==0);
		I2CONCLR=1<<5;
		
		
		I2CONCLR=1<<3;
		I2DAT=i;
		while(SI==0);
			
		delay_us(4);
		if(I2DAT==0x18)
		{
			uart0_integer(i);
			uart0_string("\r\n");
			j++;
		}
		I2CONSET=1<<4;
		I2CONCLR=1<<3;
		
	}
	uart0_string("Total slave device:");
	uart0_integer(j);
}

//=========================================================================================================		
void uart0_init(void)
{
	PINSEL0|=0x05;
	U0LCR=0x83;
	U0DLL=97;
	U0DLM=0;
	U0LCR=0x03;
}


void uart0_tx(unsigned char data)
{
	U0THR=data;
	while(THRE==0);
}

void uart0_integer (unsigned char data)
{
	uart0_tx(data/100 + 48);
	uart0_tx((data/10)%10 +48);
	uart0_tx(data%10 +48);
}

void uart0_string (char *p)
{
	while(*p)
	{
		uart0_tx(*p);
		p++;
	}
}
