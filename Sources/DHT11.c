/*
 * DHT11.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Proyecto6-01
 */
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h"
#include "DHT11.h"
unsigned int tiempo;
unsigned char j=16;
unsigned char i=16;
unsigned char l=8;
unsigned char p=0;
unsigned char z=0;
unsigned char cant_bits = 0, ini_leer = 0;
unsigned char dht11_recepbit = 0;
unsigned int pulsos_largos = 0;
unsigned int pulsos_cortos = 0;
unsigned char dht11_humedad = 0;
unsigned char dht11_temperatura = 0;
unsigned int dht11_valor1 = 0;
unsigned int dht11_valor2 = 0;
unsigned char dht11_paridad = 0;
unsigned char dht11_valor3 = 0;

void recibio_datos(void){
	if(dht11_recepbit==1 && cant_bits<19){
		
		dht11_recepbit=0;
		lee_1h();
		lee_0h();
	}
	if(dht11_recepbit==1 && cant_bits>18 && cant_bits<=34){
		
		dht11_recepbit=0;
		lee_1t();
		lee_0t();
	}
	if(dht11_recepbit==1 && cant_bits>=35){
		dht11_recepbit=0;
		lee_1p();
		lee_0p();
	}
	if(i==0 | j==0){
		calculos();
		}
 }
void lee_1p(void){
	if(tiempo<71 && tiempo>50){
		l--;
		pulsos_largos++;
		bit_paridad();
		}
	}
void lee_0p(void){
	if(tiempo<30 && tiempo>19){
		l--;
		pulsos_cortos++;
	}
}

void lee_1h (void){
	
	if(tiempo<71 && tiempo>50)
		{
				i--;
				pulsos_largos++;
				bit_1();
		}	
	}	
		
void lee_0h(void){	
	
	if(tiempo<30 && tiempo>19) {
				i--;
				pulsos_cortos++;
			}
		}
void lee_1t (void){
	if(tiempo<71 && tiempo>50){
		j--;
		pulsos_largos++;
		bit_0();
	}
}
void lee_0t(void){

	if(tiempo<30 && tiempo>19){
		j--;
		pulsos_cortos++;
	}
}

void calculos (void){
dht11_humedad = dht11_valor1 >>8;
dht11_temperatura = dht11_valor2 >>8;
}
void bit_paridad(void){

dht11_paridad = dht11_paridad | 1<<l;
}
void bit_1(void){

dht11_valor1 = dht11_valor1| 1<<i;

}
void bit_0(void){

dht11_valor2 = dht11_valor2 | 1<<j;
}

void start_sensor(void){
 PTFD_PTFD5 = 0;
 TPM2MOD = 57;
 delay_dht11();		
 PTFD_PTFD5 = 1;
 TPM2MOD = 26977;
 PTFD_PTFD5 = 0;
 delay_dht11();
 PTFD_PTFD5 = 1;
 TIMER_init2();
 }
void TIMER_init (void){
		
		TPM2MOD=29997;
		TPM2SC=0b00001100;
		}
void TIMER_init2 (void){
	
	TPM2MOD = 29997;
	TPM2SC = 0b00001011;
	TPM2C1SC = 0b01000100;
	}

__interrupt VectorNumber_Vtpm2ch1 void cont_tim (void)  {
		
			TPM2C1SC_CH1F = 0;
			if(p == 0){
			TPM2CNT = 0;
			TPM2C1SC = 0b01001000;
			p = 1;
			}
			else{
			tiempo = TPM2CNT;
			tiempo = (tiempo/3);
			dht11_recepbit = 1;
			TPM2C1SC = 0b01000100;
			p=0;
			cant_bits++;
			}	
		}
void delay_dht11 (void){
		
		while(TPM2SC_TOF==0)
			{		
			}
		TPM2SC_TOF=0;
		}

void resultados_hum (void){
	if (ini_leer == 0){
			TPM2C1SC = 0b00000000;
			PTFDD_PTFDD5=1;
			j=16;
			i=16;
			l=8;
			p=0;
			z=0;
			cant_bits=0;
			dht11_recepbit=0;
			dht11_valor1=0;
			dht11_valor2=0;
			dht11_valor3=0;
			TIMER_init();	
			start_sensor();
			ini_leer = 1;
			}
			recibio_datos();
			}
