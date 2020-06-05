#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "RTC.h"

unsigned char b, c[7], d, statreg;

/*   variable necesaria para I2C   */
unsigned char i2c_error_RTC; // bit error

unsigned char seg_dec, min_dec, horas_dec, mes_dec, anio_dec, dia_sem_dec, dia_mes_dec;
unsigned char VUmin, VDmin, VUs, VDs, VUhor, VDhor, VUdia, VDdia;

void cuentas(void){
	
	c[0] = leer_byte_rtc(0);	// segundos
	c[1] = leer_byte_rtc(1);	// minutos
	c[2] = leer_byte_rtc(2);	// horas
	c[3] = leer_byte_rtc(3);	// Dia de la semana
	c[4] = leer_byte_rtc(4);	// Dia del mes
	c[5] = leer_byte_rtc(5);	// mes
	c[6] = leer_byte_rtc(6);	// año
	
	seg_dec = bcd_to_dec(c[0]);
	min_dec = bcd_to_dec(c[1]);
	horas_dec = bcd_to_dec(c[2]);
	dia_sem_dec = bcd_to_dec(c[3]);
	dia_mes_dec = bcd_to_dec(c[4]);
	mes_dec = bcd_to_dec(c[5]);
	anio_dec = bcd_to_dec(c[6]);
	
  	VUs=seg_dec%10;
	VDs=(seg_dec%100)/10;
	
	VUmin=min_dec%10;
	VDmin=(min_dec%100)/10;
	
	VUhor=horas_dec%10;
	VDhor=(horas_dec%100)/10;
	
	VUdia=dia_mes_dec%10;
	VDdia=(dia_mes_dec%100)/10;
	
}


__interrupt VectorNumber_Vrtc void ISR_Vrtc(void){
	RTCSC_RTIF = 1;
}


void CLCK_init(void){		//Hace el clk mas preciso
    asm {
    	LDA $FFAF  ;-- inicializa osc. interno
    	STA MCGC1
    }
    while(!MCGSC_IREFST);  /* espera ck interno estable */
}

//-------------------------------------FUNCIONES I2C-----------------------


/* escribe cantidad de bytes desde dirección - para 24Cxx */
void escribir_memo_RTC(const unsigned char* buffer, unsigned int direccion, unsigned char cantidad) {
	unsigned char byte_dire;
	unsigned char i;

	/* enviar start */
	i2c_start_RTC();

	// para 24C...   A2 A1 A0 
	i2c_tx_RTC(0b10101110); //    1010        0  0  0   0 = escribir

	/* enviar address hi */
	byte_dire = direccion >> 8;
	i2c_tx_RTC(byte_dire);

	/* enviar address lo */
	byte_dire = direccion & 0x00FF;
	i2c_tx_RTC(byte_dire); // address lo

	/* enviar datos */
	for (i = 0; i < cantidad; i++) {
		i2c_tx_RTC(*buffer);
		I2Cdelay_RTC();

		buffer++;
	}

	/* enviar stop */
	i2c_stop_RTC();

	/* espera fin ciclo escritura (ACK polling) */
	do {
		i2c_start_RTC();
		// para 24C...   A2 A1 A0 
		i2c_tx_RTC(0b10101110); //    1010        0  0  0   0 = escribir				
	} while (i2c_error_RTC == 1);

}

/* lee cantidad bytes desde a direccion - para 24Cxx */
void leer_memo_RTC(unsigned char* buffer, unsigned int direccion, unsigned char cantidad) {
	unsigned char p;
	unsigned char byte_dire;

	/* enviar start */
	i2c_start_RTC();
	// para 24C...   A2 A1 A0 
	i2c_tx_RTC(0b10101110); //    1010        0  0  0   0 = escribir	

	/* enviar address hi */
	byte_dire = direccion >> 8;
	i2c_tx_RTC(byte_dire);

	/* enviar address lo */
	byte_dire = direccion & 0x00FF;
	i2c_tx_RTC(byte_dire);

	I2Cdelay_RTC();

	/* leer datos */
	p = 0;
	while (p < cantidad) {
		i2c_start_RTC();

		// para 24C...   A2 A1 A0 
		i2c_tx_RTC(0b10101111); //    1010        0  0  0   1 = leer

		p++;
		*buffer = i2c_rx_RTC(1);
		buffer++;

		i2c_stop_RTC();

		I2Cdelay_RTC();
	}
}

/* escribe byte a direccion en DS3231 */
void escribir_byte_rtc(unsigned char direccion, unsigned char dato) {

	/* enviar start */
	i2c_start_RTC();

	// para   DS3231 		 A2 A1 A0 
	i2c_tx_RTC(0b11010000); //    1101        0  0  0   0 = escribir	

	/* enviar address */
	i2c_tx_RTC(direccion);

	I2Cdelay_RTC();

	/* enviar dato */
	i2c_tx_RTC(dato);
	I2Cdelay_RTC();

	/* enviar stop */
	i2c_stop_RTC();

	/* espera fin ciclo escritura (ACK polling) */
	do {
		i2c_start_RTC();
		// para   DS3231 		 A2 A1 A0 
		i2c_tx_RTC(0b11010000); //    1101        0  0  0   0 = escribir	
	} while (i2c_error_RTC == 1);
}

/* escribe un byte a direccion - para 24Cxx */
void escribir_byte_mem_RTC(unsigned int direccion, unsigned char dato) {
	unsigned char byte_dire;

	/* enviar start */
	i2c_start_RTC();

	// para 24C...   A2 A1 A0 
	i2c_tx_RTC(0b10101110); //    1010        0  0  0   0 = escribir	

	/* enviar address hi */
	byte_dire = direccion >> 8;
	i2c_tx_RTC(byte_dire);

	/* enviar address lo */
	byte_dire = direccion & 0x00FF;
	i2c_tx_RTC(byte_dire);

	I2Cdelay_RTC();

	/* enviar dato */
	i2c_tx_RTC(dato);
	I2Cdelay_RTC();

	/* enviar stop */
	i2c_stop_RTC();

	/* espera fin ciclo escritura (ACK polling) */
	do {
		i2c_start_RTC();
		// para 24C...   A2 A1 A0 
		i2c_tx_RTC(0b10101110); //    1010        0  0  0   0 = escribir	
	} while (i2c_error_RTC == 1);
}

/* devuelve byte en direccion - para DS3231 */
unsigned char leer_byte_rtc(unsigned char direccion) {
	unsigned char aux;

	/* enviar start */
	i2c_start_RTC();
	// para DS3231		   A2 A1 A0 
	i2c_tx_RTC(0b11010000); //    1101        0  0  0   0 = escribir	

	/* enviar address */
	i2c_tx_RTC(direccion);

	I2Cdelay_RTC();

	/* enviar start */
	i2c_start_RTC();
	// para DS3231   		A2 A1 A0 
	i2c_tx_RTC(0b11010001); //    1101        0  0  0   1 = leer

	/* obtiene byte */
	aux = i2c_rx_RTC(1);

	/* enviar stop */
	i2c_stop_RTC();

	I2Cdelay_RTC();

	return aux;
}

/* devuelve byte en direccion - para 24Cxx */
unsigned char leer_byte_mem_RTC(unsigned int direccion) {
	unsigned char aux;
	unsigned char byte_dire;

	/* enviar start */
	i2c_start_RTC();
	// para 24C...   A2 A1 A0 
	i2c_tx_RTC(0b10101110); //    1010        0  0  0   0 = escribir	

	/* enviar address hi */
	byte_dire = direccion >> 8;
	i2c_tx_RTC(byte_dire);

	/* enviar address lo */
	byte_dire = direccion & 0x00FF;
	i2c_tx_RTC(byte_dire);

	I2Cdelay_RTC();

	/* enviar start */
	i2c_start_RTC();
	// para 24C...   A2 A1 A0 
	i2c_tx_RTC(0b10101111); //    1010        0  0  0   1 = leer

	/* obtiene byte */
	aux = i2c_rx_RTC(1);

	/* enviar stop */
	i2c_stop_RTC();

	I2Cdelay_RTC();

	return aux;
}

/* --- funciones generales para IIC --- */
void I2Cdelay_RTC(void) {
	unsigned char i;

	for (i = 0; i < 10; i++);
}

void i2c_tx_RTC(unsigned char byte) {
	unsigned char i;

	DDR_SDA2 = salida; // SDA salida

	for (i = 0; i < 8; i++) {
		if (byte & 0x80)
			SDA2 = 1; //envia cada `, comenzando por el MSB
		else
			SDA2 = 0;

		SCL2 = 1;

		I2Cdelay_RTC();

		SCL2 = 0;

		I2Cdelay_RTC();

		byte = byte << 1;
	}

	DDR_SDA2 = entrada; // SDA entrada ; espera ACK (config. pin como lectura)

	SCL2 = 1;

	I2Cdelay_RTC();

	if (SDA2)
		i2c_error_RTC = 0x01; // error si el ACK no es correcto
	else
		i2c_error_RTC = 0;

	SCL2 = 0;

	I2Cdelay_RTC();
}

void i2c_start_RTC() {
	DDR_SDA2 = salida; // SDA salida

	SDA2 = 1; // garantizamos el estado inicial de SDA y SCL (reposo)

	SCL2 = 1;

	I2Cdelay_RTC();

	SDA2 = 0; // condicion START

	I2Cdelay_RTC();

	SCL2 = 0;

	I2Cdelay_RTC();
}

void i2c_addr_RTC(unsigned char addr, unsigned char rw) {

	// addr, dirección (7 bits)
	// rw=1, lectura
	// rw=0, escritura

	i2c_tx_RTC(addr << 1 + rw); // envia byte de direccion
}

unsigned char i2c_rx_RTC(unsigned char ultimo)
// ultimo = NACK = 1 para el ultimo byte
// ultimo = ACK = 0 para cualquier byte que no sea el último.
{
	unsigned char i, byte;

	byte = 0;

	DDR_SDA2 = entrada; //SDA entrada

	for (i = 0; i < 8; i++) // lee un bit comenzando por el MSB
			{
		//SCL=1;

		do {
			DDR_SCL2 = salida;
			SCL2 = 1;
			DDR_SCL2 = entrada;
		} while (SCL2 == 0); // SCL ck stretching?

		DDR_SCL2 = salida;

		I2Cdelay_RTC();

		byte <<= 1;

		if (SDA2)
			byte++;

		SCL2 = 0;

		I2Cdelay_RTC();
	}

	DDR_SDA2 = salida; // SDA salida

	SDA2 = ultimo; // ACK es funcion de ultimo

	SCL2 = 1; // ACK=0, si no es último byte leído

	I2Cdelay_RTC(); // ACK=1, después del último byte leído (NACK)

	SCL2 = 0;

	I2Cdelay_RTC();

	return (byte);
}

void i2c_stop_RTC(void) {
	DDR_SDA2 = salida; // SDA salida

	SDA2 = 0;

	I2Cdelay_RTC();

	SCL2 = 1;

	I2Cdelay_RTC();

	SDA2 = 1;

	I2Cdelay_RTC();
}

unsigned char bcd_to_dec(unsigned char d) {
return ((d & 0x0F) + (((d & 0xF0) >> 4) * 10));
}



//Cada 1mSeg
void init_RTC(void){
	RTCMOD = 0; 
	RTCSC  = 0x18;   //Interrupcion Encendida, cada 1mSeg
}

// DELAY - with an 8.0MHz bus.
void delay(unsigned int n)
{

	//2uSeg Aprox
	n = n*3;
	n=n/2;
  asm {
       LDHX (n)
LOOP1: AIX  #-1    ;[2]
       NOP
       NOP 
       CPHX #0     ;[3]
       BNE  LOOP1  ;[3]
  }
}
