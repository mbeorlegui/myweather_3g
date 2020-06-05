/*
 * DHT11.h
 *
 *  Created on: Nov 2, 2017
 *      Author: Proyecto6-01
 */

#ifndef DHT11_H_
#define DHT11_H_

/*SENSOR HUMEDAD Y TEMPERATURA */
void TIMER_init (void);
void TIMER_init2 (void);
void start_sensor(void);
void leer_sensor(void);
void delay_dht11 (void);
void bit_1(void);
void bit_0(void);
void lee_1h(void);
void lee_0h(void);
void calculos(void);
void lee_1t(void);
void lee_0t(void);
void lee_1p(void);
void lee_0p(void);
void bit_paridad(void);
void recibio_datos (void);
void resultados_hum(void);



#endif /* DHT11_H_ */
