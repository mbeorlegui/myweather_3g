#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h"
#include "Radiacion.h"

unsigned int UV;
unsigned int vref;
unsigned char estado_adc=1; 

/* SENSOR UV */
	void ADC_init1 (void){
	
		SPMSC1_BGBE = 1; 
		ADCCFG = 0b00001000; // fadc = fbus / 8 = 1 MHz; 10 bits
		ADCSC2 = 0;
		APCTL2_ADPC8 = 1; // pin 8 Analogico
	}
	void medicion_uv(void){
		 switch( estado_adc ) {
				case INICIO_BG:
				 
				   ADCSC1 = 0b01111011;  
				   estado_adc = ESPERO_COCO_BG;
				   break;
					
				case ESPERO_COCO_BG:
					if (ADCSC1_COCO == 1) {
						estado_adc = INICIO_VI;
						vref = ADCR;
					}
					break;
				
				case INICIO_VI:
					ADCSC1 = 0b00001000; 
					estado_adc = ESPERO_COCO_VI;
					break;
				
				case ESPERO_COCO_VI:
					if (ADCSC1_COCO == 1) {
						estado_adc = INICIO_BG;
						UV = ADCR;           
					break;
				}
			}
	}
