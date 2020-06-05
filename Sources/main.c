#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "I2C.h"
#include "LCD_I2C.h"
#include "CLK.h"
#include "Radiacion.h"
#include "DHT11.h"
#include "BMP180.h"
#include "RTC.h"
#include "GSM.h"
#include "SCI.h"
#include "TPM.h"

void MCU_init (void); //inicializa MCU
void PORT_init(void); //inicializa ports

void muestreo_datos (void);
void muestreo (void);
void muestra_DIA(void);
void muestra_FECHA(void);
void muestra_HORA(void);
void limpia_pantalla(void);
unsigned char sensores_gen=0;
unsigned char e=0;

void TPM1_init(void);		 
unsigned char time_out=0;
extern unsigned int UV;
extern unsigned char dht11_humedad;
extern unsigned char dht11_temperatura;
extern unsigned char ini_leer;
extern long pres_cal;
extern long temp_cal;
extern long pres_decimal;
extern long temp_entero;
extern long temp_decimal;
extern long pres_entero;
extern unsigned char seg_dec, min_dec, horas_dec, mes_dec, anio_dec, dia_sem_dec, dia_mes_dec;

void main(void) {
	asm {
			LDHX #0x10AE  
			TXS ;SP<-(H:X-1)
		}
	MCU_init ();
	CLK_Init();
	PORT_init(); 
	ADC_init1();
	registros_bmp180();
	TPM1_init(); 
	Init_IIC();	
	LCD_init();
	_LED_HEART=GPIO_OUT;
	_LED_GSM  =GPIO_OUT;
	TIMER2_Init();
	SCI2_Init();
	GSM_init();
	mostrar_mensaje (0x80, "     WELCOME TO         ");
	mostrar_mensaje (0xC0, "     MY WEATHER         ");
	mostrar_mensaje (0x94, "      GRUPO 08          ");
	mostrar_mensaje (0xD4, "   COLEGIO PIO IX       ");
	EnableInterrupts;
	
	for(;;) {
				cuentas();
				GSM_step();
		 switch(sensores_gen){
					
					 case 0:
						resultados_hum();
						break;
					 case 1:
						//medicion_uv();
						break;
					 case 2:
						resultados_pres_temp();
						break;
					 case 3:
						temp_entero=(temp_cal/10)-5;
						temp_decimal=temp_cal%10;
						pres_entero=(pres_cal/100)-15;
						pres_decimal=pres_cal%100;
						break;
					 case 4:
						 if(e==0){
							limpia_pantalla();
							}
						else{
							muestreo_datos();
						}
						 break;
					 case 5:	 
						 muestreo();
						 if(e==0){
						 sensores_gen=0;
						 e++;
						 }
						 break;
					 case 6:
						  limpia_pantalla();
						  muestra_DIA();
						  muestra_FECHA();
						  muestra_HORA();
						  e=0;
						  sensores_gen=0;
						  break;
					
					}
			}
	}

void limpia_pantalla(void){
	mostrar_mensaje (0x94, "                           ");
	mostrar_mensaje (0xD4, "                           ");
	mostrar_mensaje (0x80, "                           ");
	mostrar_mensaje (0xC0, "                           ");
}
void muestreo (void){
	 mostrar_mensaje(0x80, "TEMPERATURA:");
	 mostrar_mensaje(0x8F,".");
	 mostrar_mensaje(0x92, "C");
	 mostrar_mensaje(0xD4, "HUMEDAD:");
	 mostrar_mensaje(0xDF, "%");
	 mostrar_mensaje(0xC0, "PRESION:");
	 mostrar_mensaje (0xCD, ".");
	 mostrar_mensaje(0xD0, "hPa");
	 mostrar_mensaje(0x94, "Indice UV:");
	 mostrar_mensaje(0xA3, "W/m2");
}
void muestreo_datos(void){
	mostrar_mensajeint(0x8D, temp_entero);
	mostrar_mensajeint(0x90, temp_decimal);
	mostrar_mensajeint(0xDD, dht11_humedad);
	mostrar_mensajeint(0xC9, pres_entero/10);
	mostrar_mensajeint(0xCB, (pres_entero%100)/10);
	mostrar_mensajeint(0xCC, pres_entero%10);
	mostrar_mensajeint(0xCE, pres_decimal);
	mostrar_mensajeint(0x9F, UV);
}  
void muestra_DIA(void){
	switch (dia_sem_dec){
			case 0:
				break;
			case 1:
				mostrar_mensaje(0xC6,"DOM ");
				break;
			case 2:
				mostrar_mensaje(0xC6,"LUN ");
				break;
			case 3:
				mostrar_mensaje(0xC6,"MAR ");
				break;
			case 4:
				mostrar_mensaje(0xC6,"MIE ");
				break;
			case 5:
				mostrar_mensaje(0xC6,"JUE ");
				break;
			case 6:
				mostrar_mensaje(0xC6,"VIE ");
				break;
			case 7:
				mostrar_mensaje(0xC6,"SAB ");
				break;
			default:
				break;
	}
}
void muestra_FECHA(void){
		mostrar_mensajeint(0xCA, dia_mes_dec);
		mostrar_mensaje(0xCC, "/");
		mostrar_mensajeint(0xCD, mes_dec);
		mostrar_mensaje(0xCF, "/");
		mostrar_mensajeint(0xD0, anio_dec);
}
void muestra_HORA(void){
		mostrar_mensajeint(0x9D, horas_dec);
		mostrar_mensaje(0x9F, ":");
		mostrar_mensajeint(0xA0, min_dec);
}
void MCU_init(void) {

	SOPT1 = SOPT1 & 0x3F; //deshabilito COP
}
__interrupt VectorNumber_Vtpm1ovf void tim_ovf(void){
	
	TPM1SC_TOF=0;
	time_out++;
	
	if(time_out==50){
		time_out=0;	
		sensores_gen++;
		ini_leer=0;
	}
}                  
void PORT_init(void) {
	/* -- GPIOs en 0 -- */
	PTAD = 0;
	PTBD = 0;
	PTCD = 0;
	PTDD = 0;
	PTED = 0;
	PTFD = 0;
	PTGD = 0;
	/* -- Configuración E-S de GPIO -- */
	PTADD = 0b00000000;
	PTBDD = 0b00000000;
	PTCDD_PTCDD3=1;
	PTCDD_PTCDD5=1;
	PTDDD = 0b00000000;
	PTEDD = 0b00000000;
	PTFDD_PTFDD5 = 1;
	PTGDD = 0b00000000;
}
void TPM1_init() {
	TPM1MOD = 18749;
	TPM1SC = 0b01001111; // TOIE=1, TCKSRC = BUS DIV=1
}
