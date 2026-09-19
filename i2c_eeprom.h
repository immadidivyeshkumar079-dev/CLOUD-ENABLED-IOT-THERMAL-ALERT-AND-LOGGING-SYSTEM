#ifndef I2C_EEPROM_H
#define I2C_EEPROM_H


#include "types.h"


/*
 * Write single byte to EEPROM
 */
void i2c_eeprom_bytewrite(u8 slaveAddr,u16 wBuffAddr,u8 dat);


/*
 * Read single byte from EEPROM
 */
u8 i2c_eeprom_randomread(u8 slaveAddr,u16 rBuffAddr);


/*
 * Write multiple bytes to EEPROM page
 */
void i2c_eeprom_pagewrite(u8 slaveAddr,u16 wBuffStartAddr,u8 *p,u8 nBytes);


/*
 * Read multiple bytes sequentially from EEPROM
 */
void i2c_eeprom_seqread(u8 slaveAddr,u16 rBuffStartAddr, u8 *p,u8 nBytes);

/* I2C EEPROM 7-bit slave address */
#define EEPROM_SLAVE_ADDR 0x50
#endif
