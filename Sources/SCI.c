#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "SCI.h"
#include "GSM.h"

#define	BUS_CLK       24000000ul	/* bus frequency in Hz */

unsigned char sci1_data=0,sci2_data=0;

void SCI1_Init(void)
{
	SCI1BD = SCI_CalcBaudDivisor(115200);
 	SCI1C1 = 0;		
	
	SCI1C2 = 0b00101100;
				//Bit 7 	TIE 	0		  Transmit Interrupt Enable
				//Bit 6 	TCIE	0 		Transmission Complete Interrupt Enable
				//Bit 5 	RIE		1 		Receiver Interrupt Enable
				//Bit 4 	ILIE	0 		Idle Line Interrupt Enable
				//Bit 3 	TE		1 		Transmitter Enable
				//Bit 2 	RE		1 		Receiver Enable
				//Bit 1 	RWU		0 		Receiver Wakeup Control
				//Bit 0 	SBK		0 		Send Break
	SCI1C3 = 0x00;	
}

void SCI2_Init(void)
{
	SCI2BD = SCI_CalcBaudDivisor(115200);
 	SCI2C1 = 0;		
	
	SCI2C2 = 0b00101100;
				//Bit 7 	TIE 	0		Transmit Interrupt Enable
				//Bit 6 	TCIE	0 		Transmission Complete Interrupt Enable
				//Bit 5 	RIE		1 		Receiver Interrupt Enable
				//Bit 4 	ILIE	0 		Idle Line Interrupt Enable
				//Bit 3 	TE		1 		Transmitter Enable
				//Bit 2 	RE		1 		Receiver Enable
				//Bit 1 	RWU		0 		Receiver Wakeup Control
				//Bit 0 	SBK		0 		Send Break
	SCI2C3 = 0x00;	
}

void SCI1_Send_Byte(unsigned char data)
{
	//Esperamos que se libere la UART
	while(SCI1S1_TC==0){};
	SCI1D=data;
}

void SCI1_Send_Buffer(unsigned char *buffer, unsigned char cantidad)
{
	while(cantidad)
	{
		SCI1_Send_Byte(*buffer);
		buffer++;
		cantidad--;  
	}
}

void SCI1_Print(unsigned char *buffer)
{
	while(*buffer)
	{
		SCI1_Send_Byte(*buffer);
		buffer++;
	}
}

void SCI2_Send_Byte(unsigned char data)
{
	//Esperamos que se libere la UART
	while(SCI2S1_TC==0){};
	SCI2D=data;
	while(SCI2S1_TDRE == 0);
}

void SCI2_Send_Buffer(unsigned char *buffer, unsigned char cantidad)
{
	while(cantidad)
	{
		SCI2_Send_Byte(*buffer);
		buffer++;
		cantidad--;  
	}
}

void SCI2_Print(unsigned char *buffer)
{
	while(*buffer)
	{
		SCI2_Send_Byte(*buffer);
		buffer++;
	}
}

__interrupt 20 void SCI1_Rx (void){
	unsigned char sci_data_read;
	//Damos ACK a la interrupcion  
	sci_data_read=SCI1S1;
	sci_data_read=SCI1C3;
	sci_data_read=SCI1D;
}

__interrupt 23 void SCI2_Rx (void){
	unsigned char sci_data_read;
	//Damos ACK a la interrupcion  
	sci_data_read=SCI2S1;
	sci_data_read=SCI2C3;
	sci_data_read=SCI2D;
	
	InDataRxBuffer(sci_data_read);
	
}

unsigned int SCI_CalcBaudDivisor (unsigned long BaudRate){
    unsigned int div_br=0;
    
    div_br = (unsigned int)((BUS_CLK/16)/BaudRate);
    if (div_br>8192)	div_br=8192;
    if (div_br<1)		div_br=1;
    
    return div_br;   
}
