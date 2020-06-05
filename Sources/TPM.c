#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "TPM.h"
#include "GSM.h"
#include "hardware.h"


unsigned int tout_500mSeg=0;

//Inicializa el timer cada 1mSeg
void TIMER2_Init(void){
	TPM2SC  =0;
	TPM2MOD =3000;	
	TPM2SC  =0b01001011; // TOIE=1, TCKSRC=BUS DIV=8
}


/* Interrupciones */
__interrupt 18 void TIMER2(void)
{
	TPM2SC_TOF=0;
	
	GSM_tic ();
	
	//HEART bit cada 500mSeg en LED ubicado en la placa PIOJM
	if(tout_500mSeg) tout_500mSeg--;
	else {LED_HEART=~LED_HEART;tout_500mSeg=500;}
}
