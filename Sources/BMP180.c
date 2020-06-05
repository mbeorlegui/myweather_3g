/*
 * BMP180.c
 *
 *  Created on: Nov 4, 2017
 *      Author: Marcelo
 */
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h"
#include "BMP180.h"

/* VARIABLE NECESEARIA PARA I2C */
unsigned char i2c_error, pm;


unsigned int ppp;
int AC1 = 408; //8250;
int AC2 = -72; //-1171;
int AC3 = -14383; //-14601;
unsigned int AC4 = 32741; //33645;
unsigned int AC5 = 32757; //25575;
unsigned int AC6 = 23153; //16042;
int B1 = 6130; //6515;
int B2 = 4; //46;
//int MB  = -32768;
int MC = -8711; //-11786;
int MD = 2868; //2470;
long temp_raw = 27898; //24468;
long pres_raw = 23843; //42046;
unsigned long temp,pres;
long X1;
long X2;
long X3;
long B3, B5, B6;
unsigned long B4, B7;		
long temp_cal;
long pres_cal;
long pres_entero;
long pres_decimal;
long temp_entero;
long temp_decimal;

/* SENSOR BMP180 */

void resultados_pres_temp (void){
			/*TEMPERATURA NO REAL*/
			escribir_byte(0xF4, 0x2E);
			I2Cdelay();

			for (ppp = 0; ppp < 5000; ppp++);

			temp_raw = leer_byte(0xF6);

			/*PRESION NO REAL*/
			escribir_byte(0xF4, 0x34);
			I2Cdelay();

			for (ppp = 0; ppp < 5000; ppp++);
			pres_raw = leer_byte(0xF6);
								
								
			temp = calcula_temp();
			pres = calcula_pres();
		 
	 }
		

void registros_bmp180(void){

	DDR_SDA = salida;
	DDR_SCL = salida;
	AC1 = 8250;
	AC2 = -1171;
	AC3 = -14601;
	AC4 = 33645;
	AC5 = 25575;
	AC6 = 16042;
	B1 = 6515;
	B2 = 46;
	MC = -11786;
	MD = 2470;
	temp_raw = 24468;
	pres_raw = 42046;	
}
long calcula_temp(){
	
	X1 = ((temp_raw - AC6) * AC5) / 32768L;
	X2 = (MC * (2048L)) / (X1 + MD);
	B5 = X1 + X2;
	temp_cal = (B5 + 8) / 16L;
	return temp_cal;
}

long  calcula_pres(){
	long aux;
	/* ------------------------------------------------- */
	B6 = B5 - 4000;
	
	aux = (B6 * B6 / 4096L);
	
	X1 = (B2 * aux) / 2048L;
	X2 = AC2 * B6 / 2048L;
	X3 = X1 + X2;

	aux = ((long) AC1 * 4 + X3);
	
	B3 = (aux + 2) / 4;
	
	X1 = (AC3 * B6) / (8192L);
	
	aux = B6 * B6 / (4096L);
	X2 = (B1 * (aux)) / 65536;
	X3 = ((X1 + X2) + 2) / 4;

	B4 = AC4 * (unsigned long) (X3 + 32768) / (1L << 15);
	B7 = ((unsigned long) pres_raw - B3) * (50000 >> 0);
	if (B7 < 0x80000000) {
		pres_cal = (B7 * 2) / B4;
	} else {
		pres_cal = (B7 / B4) * 2;
	}
	X1 = (pres_cal / (1L << 8)) * (pres_cal / (1L << 8));

	X1 = (X1 * 3038) / (1L << 16);
	X2 = (-7357 * pres_cal) / (1L << 16);
	pres_cal = pres_cal + (X1 + X2 + 3791) / (1L << 4);
	return pres_cal;
}


void escribir_byte(unsigned int direccion, unsigned char dato) {
	unsigned char byte_dire;
	
	/* enviar start */
	i2c_start();
	// para 24C...   A2 A1 A0 
	i2c_tx(0xEE); //    1010        0  0  0   0 = escribir y cambiar id menos el ultimo bit que es el de leer o escribir	
	
	/* enviar address lo */
	byte_dire = direccion & 0x00FF;
	i2c_tx(byte_dire);
	
	I2Cdelay();
	
	/* enviar dato */
	i2c_tx(dato);
	I2Cdelay();
	
	/* enviar stop */
	i2c_stop();
	
	/* espera fin ciclo escritura (ACK polling) */
	do {
		i2c_start();
		// para 24C...   A2 A1 A0 
		i2c_tx(0xEE); //    1010        0  0  0   0 = escribir y cambiar id menos el ultimo bit que es el de leer o escribir	
	} while (i2c_error == 1);
}

unsigned int leer_byte(unsigned int direccion) {
	unsigned char byte_dire;
	unsigned int auxLo, auxHi;
	
	/* enviar start */
	i2c_start();
	// para 24C...   A2 A1 A0 
	i2c_tx(0xEE); //    1010        0  0  0   0 = escribir y cambiar id menos el ultimo bit que es el de leer o escribir	
	
	/* enviar address lo */
	byte_dire = direccion & 0x00FF;
	i2c_tx(byte_dire);
	
	I2Cdelay();
	
	/* enviar start */
	i2c_start();
	// para 24C...   A2 A1 A0 
	i2c_tx(0xEF); //    1010        0  0  0   1 = leer y cambiar id menos el ultimo bit que es el de leer o escribir 
	
	/* obtiene byte */
	auxHi = i2c_rx(0);
	
	/* obtiene byte */
	auxLo = i2c_rx(1);
	
	/* enviar stop */
	i2c_stop();
	
	I2Cdelay();
	
	return (auxHi << 8) | auxLo;
}

void I2Cdelay(void) {
	unsigned char i;
	
	for (i = 0; i < 200; i++);
}

void i2c_tx(unsigned char byte) {
	unsigned char i;
	
	DDR_SDA = salida; // SDA salida
	
	for (i = 0; i < 8; i++) {
		if (byte & 0x80)
			SDA = 1; //envia cada bit, comenzando por el MSB
		else
			SDA = 0;
	
		SCL = 1;
	
		I2Cdelay();
	
		SCL = 0;
	
		I2Cdelay();
	
		byte = byte << 1;
	}
	
	DDR_SDA = entrada; // SDA entrada ; espera ACK (config. pin como lectura)
	
	SCL = 1;
	
	I2Cdelay();
	
	if (SDA)
		i2c_error = 0x01; // error si el ACK no es correcto
	else
		i2c_error = 0;
	
	SCL = 0;
	
	I2Cdelay();
}

void i2c_start() {
	DDR_SDA = salida; // SDA salida
	
	SDA = 1; // garantizamos el estado inicial de SDA y SCL (reposo)
	
	SCL = 1;
	
	I2Cdelay();
	
	SDA = 0; // condicion START
	
	I2Cdelay();
	
	SCL = 0;
	
	I2Cdelay();
}

void i2c_addr(unsigned char addr, unsigned char rw) {
	
	// addr, dirección (7 bits)
	// rw=1, lectura
	// rw=0, escritura
	
	i2c_tx(addr << 1 + rw); // envia byte de direccion
	}

unsigned char i2c_rx(unsigned char ultimo)
	// ultimo = NACK = 1 para el ultimo byte
	// ultimo = ACK = 0 para cualquier byte que no sea el último.
	{
	unsigned char i, byte;
	
	byte = 0;
	
	DDR_SDA = entrada; //SDA entrada
	
	for (i = 0; i < 8; i++) // lee un bit comenzando por el MSB
			{
		//SCL=1;
	
		do {
			DDR_SCL = salida;
			SCL = 1;
			DDR_SCL = entrada;
		} while (SCL == 0); // SCL ck stretching?
	
		DDR_SCL = salida;
	
		I2Cdelay();
	
		byte <<= 1;
	
		if (SDA)
			byte++;
	
		SCL = 0;
	
		I2Cdelay();
	}
	
	DDR_SDA = salida; // SDA salida
	
	SDA = ultimo; // ACK es funcion de ultimo
	
	SCL = 1; // ACK=0, si no es último byte leído
	
	I2Cdelay(); // ACK=1, después del último byte leído (NACK)
	
	SCL = 0;
	
	I2Cdelay();
	
	return (byte);
}

void i2c_stop(void) {
	DDR_SDA = salida; // SDA salida
	
	SDA = 0;
	
	I2Cdelay();
	
	SCL = 1;
	
	I2Cdelay();
	
	SDA = 1;
	
	I2Cdelay();
}

