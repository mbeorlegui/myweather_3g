/*
 * LCD.h
 *
 *  Created on: 29/08/2013
 *      Author: Alumno
 */

#ifndef LCD_I2C_H_
#define LCD_I2C_H_

void delay_lcd(unsigned int);
void LCD_init(void);
void LCD_wr(unsigned char);
void LCD_send(unsigned int, unsigned int);
void mostrar_mensajeint(unsigned int p, signed int numero);
void mostrar_mensaje(unsigned int p, unsigned char* s);

#endif /* LCD_H_ */
