/*
 * BMP180.h
 *
 *  Created on: Nov 4, 2017
 *      Author: Marcelo
 */

#ifndef BMP180_H_
#define BMP180_H_

/* SENSOR PRESION */
#define SDA PTCD_PTCD5 
#define SCL PTCD_PTCD3  
#define salida 1
#define entrada 0

#define DDR_SDA PTCDD_PTCDD5
#define DDR_SCL PTCDD_PTCDD3 

/*  FUNCIONES I2C   */
void I2Cdelay(void);
void i2c_tx(unsigned char byte);
void i2c_addr(unsigned char addr, unsigned char rw);
unsigned char i2c_rx(unsigned char ultimo);
void i2c_stop(void);
void i2c_start(void);
void registros_bmp180(void);
void resultados_pres_temp (void);
long calcula_temp();
long calcula_pres();

/* LECTURA Y ESCRITURA DE UN BYTE*/
unsigned int leer_byte(unsigned int addr);
void escribir_byte(unsigned int direccion, unsigned char dato);


#endif /* BMP180_H_ */
