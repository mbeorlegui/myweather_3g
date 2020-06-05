#ifndef _GSM_H_
#define _GSM_H_

void GSM_init(void);

#define LENMAX_BUFF 256

//Estructura donde llegan los datos
typedef struct  s_buf_serial  {
  unsigned char pIn;
  unsigned char Buffer [LENMAX_BUFF];
}t_buf_serial;


void InDataRxBuffer(unsigned char dato);
void GSM_tic (void);
void GSM_step(void);
void EnviarDatos(void);
void DirecViento(void);
void EnvVar(unsigned char);

#endif /* _GSM_H_ */
