#ifndef I2C_H_
#define I2C_H_

void Init_IIC(void);
void IIC_write_block(unsigned char, unsigned char);
unsigned char IIC_read_block (unsigned char address);

#endif 
