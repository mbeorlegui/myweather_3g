#ifndef RTC_H_
#define RTC_H_

#define SDA2 PTFD_PTFD1
#define SCL2 PTFD_PTFD0

#define DDR_SDA2 PTFDD_PTFDD1 // bit config. E/S pin SDA
#define DDR_SCL2 PTFDD_PTFDD0 // bit config. E/S pin SDA

#define salida 1
#define entrada 0

/*   funciones I2C   */
void I2Cdelay_RTC(void);
void i2c_tx_RTC(unsigned char byte);
void i2c_addr_RTC(unsigned char addr, unsigned char rw);
unsigned char i2c_rx_RTC(unsigned char ultimo);
void i2c_stop_RTC(void);
void i2c_start_RTC(void);

/*   funciones para lectura / escritura de conjunto de bytes (sequential read / write)  */
void leer_memo_RTC(unsigned char* buffer, unsigned int direccion, unsigned char cantidad);
void escribir_memo_RTC(const unsigned char* buffer, unsigned int direccion,unsigned char cantidad);

/*   funciones para lectura / escritura de un bytes (random read / write) */
unsigned char leer_byte_mem_RTC(unsigned int addr);
void escribir_byte_mem_RTC(unsigned int direccion, unsigned char dato);
unsigned char leer_byte_rtc(unsigned char addr);
void escribir_byte_rtc(unsigned char direccion, unsigned char dato);

unsigned char bcd_to_dec(unsigned char);
void init_RTC(void);
void cuentas(void);


#endif /* RTC_H_ */
