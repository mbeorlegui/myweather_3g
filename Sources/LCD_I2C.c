#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h"
#include "I2C.h"
#include "LCD_I2C.h"

#define DATO 			1
#define CMND 			0
#define DELY 			150

#define lcd 78 // Address	

extern unsigned char salida_lcd = 0b000001000;

void delay_lcd(unsigned int n){ /* -------------------------------------------------------- retardo LCD --- */
	asm{
		LDHX n
	_seguir: AIX #-1
		CPHX #0
		BNE _seguir
	}
}

void LCD_init(void){ /* ------------------------------------------------- inicialización LCD --- */
	delay_lcd(50000);
	delay_lcd(50000);
	delay_lcd(50000);

		LCD_wr(0x03);
		LCD_wr(0x03);
		LCD_wr(0x03);
	  
	  /* --- modo 4 bit --- */
	  LCD_wr(0x02);
	  
	  /* --- modo 4b - 2 lin - car 5x7 --- */
	  LCD_send(0x28,CMND);
	  
	  /* --- apaga display --- */
	  LCD_send(0x08,CMND);
	  
	  /* --- Clear --- */
	  LCD_send(0x01,CMND);
	  delay_lcd(50000);
	  
	  /* --- disp on - cur off - blink off --- */
	  LCD_send(0x0C,CMND);
	  
	  /* --- inc - no scroll --- */
	  LCD_send(0x06,CMND);

 }

void LCD_wr(unsigned char data){
	/* ----------------------------------------------------- escribe 4 bits --- */
	/* --------------------------------------------------- en patas B[3..0] --- */
	

	if (data & 0x08) salida_lcd = salida_lcd | 0b10001000; //1 a D7
	else salida_lcd = salida_lcd & 0b01111111; //0 a D7

	if (data & 0x04) salida_lcd = salida_lcd | 0b01001000; //1 a D6
	else salida_lcd = salida_lcd & 0b10111111; //0 a D6 

	if (data & 0x02) salida_lcd = salida_lcd | 0b00101000; //1 a D5
	else salida_lcd = salida_lcd & 0b11011111; //0 a D5

	if (data & 0x01) salida_lcd = salida_lcd | 0b00011000; //1 a D4
	else salida_lcd = salida_lcd & 0b11101111; //0 a D4
	
	IIC_write_block (lcd, salida_lcd);
	
	

	salida_lcd = salida_lcd | 0b00000100; // E = 1;
	IIC_write_block (lcd, salida_lcd);

	salida_lcd = salida_lcd & 0b11111011; // E = 0;
	IIC_write_block (lcd, salida_lcd);
	
	delay_lcd(DELY);

}
 
void LCD_send(unsigned int data, unsigned int ope) {/* -------------- escribe 4 MSb, luego 4 LSb Si ope = 1 DATO, si 0 CMND --- */
	 unsigned char half;
	
	 if (ope != 0)salida_lcd = salida_lcd | 0b00001001; // RS = 1;
	 else salida_lcd = salida_lcd & 0b11111110; //RS = 0;
	 IIC_write_block (lcd, salida_lcd);
	 
	
	 half = data & 0xF0;
	 half = half >> 4;
	 
	 LCD_wr(half);
	
	 half = data & 0x0F;
	
	 LCD_wr(half);
	
	 salida_lcd = salida_lcd & 0b11111110; //RS = 0;
	 IIC_write_block (lcd, salida_lcd);
}

void mostrar_mensajeint (unsigned int p, signed int numero){
  int resto;
  unsigned int datos[10];
  int j = 0;
  

  LCD_send(p,0);	 
  
  	  if (numero == 0)LCD_send(0b00110000,DATO);	  
   
  	  if (numero < 0){
  		  LCD_send(0b00101101,DATO); // muestra el -
  		  numero = numero * (-1);
  	  }
  

	while(numero > 0){  
		resto=numero % 10;
		datos[j]= resto;
		numero=numero / 10;  
		j++;
	}
  
	j--;
  
	while (j >= 0){
		LCD_send(0b00110000 + (datos[j]),DATO);
		j--;
	}
  
}

void mostrar_mensaje (unsigned int p, unsigned char* s){
	 
 int i = 0;
 
 LCD_send(p,0);	 

 while(s[i] != '\0'){	 
		LCD_send(s[i],DATO);
		i++;
	} 

}


