#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h"
#include "I2C.h"

void Init_IIC(void){
	IICC1_IICEN = 1;     // Enable I2C
	IICC1_TXAK = 1;      // el master no genera ACK luego de transmitir
	IICC1_MST = 0;
	IICF = 0b01011111; //mult= x2, ICR=0x1F, MULT=240, Baud rate=50Kbits/s
	IICS_SRW = 0;       // R/W bit = 0;
	
}

void IIC_write_block(unsigned char address, unsigned char data){

	IICC1_IICEN = 1;    	// Enable I2C
	
	IICC1_TXAK = 0;         // RX/TX = 1; MS/SL = 1; TXAK = 0;
	IICC1 |= 0x30;          // condicion de START
			
	//------- Transmisión del Slave Address al IIC Bus -------
	IICD = address & 0xFE; 		// Slave Address y 0 en R/W bit (Para escribir)
	while (!IICS_IICIF);   		// esperamos que termine la Transmisión
	IICS_IICIF = 1;          		// Limpiamos el flag de interrupcion
	while(IICS_RXAK);      		// Acknowledge recibido
	
	//------- Transmisión del dato a escribir en el registro al IIC Bus -------
	IICD = data;           		// Dato a escribir
	while (!IICS_IICIF);
	IICS_IICIF = 1;
	while(IICS_RXAK);
	
	IICS_IICIF = 1;
	
	IICC1_MST = 0;          // generamos la condicion de STOP
	
	IICC1_IICEN = 0;        // Disable I2C
	
}

unsigned char IIC_read_block (unsigned char address){
	unsigned char respuesta;
	
	IICC1_IICEN = 1;    	// Enable I2C
		
	IICC1_TXAK = 0;         // RX/TX = 1; MS/SL = 1; TXAK = 0;
	IICC1 |= 0x30;          // condicion de START
			
	//------- Transmisión del Slave Address al IIC Bus -------
	IICD = address & 0xFE; 		// Slave Address y 0 en R/W bit (Para escribir)
	while (!IICS_IICIF);   		// esperamos que termine la Transmisión
	IICS_IICIF = 1;          		// Limpiamos el flag de interrupcion
	while(IICS_RXAK);      		// Acknowledge recibido
	
	//------- Recepcion de datos-------
	IICC1_MST = 1;          // generamos la condicion de STOP
	
	
	respuesta = IICD;
	
	IICC1_MST = 0;          // generamos la condicion de STOP
	
	IICC1_IICEN = 0;        // Disable I2C
	
	return respuesta;
	
}

