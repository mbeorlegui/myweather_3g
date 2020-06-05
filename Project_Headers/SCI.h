#ifndef _SCI_H_
#define _SCI_H_

extern unsigned char sci1_data,sci2_data;

void SCI1_Init(void);
void SCI1_Send_Byte(unsigned char data);
void SCI1_Send_Buffer(unsigned char *buffer, unsigned char cantidad);
void SCI1_Print(unsigned char *buffer);
void SCI2_Init(void);
void SCI2_Send_Byte(unsigned char data);
void SCI2_Send_Buffer(unsigned char *buffer, unsigned char cantidad);
void SCI2_Print(unsigned char *buffer);
unsigned int SCI_CalcBaudDivisor (unsigned long BaudRate);

#endif /* SCI_H_ */
