#include "types.h"
#include "delay.h"
#include "i2c_peripheral.h"


/*
 * Write single byte to EEPROM
 */
void i2c_eeprom_bytewrite(u8 slaveAddr, u16 wBuffAddr, u8 dat)
{
    // Start condition
    i2c_start();

    // Slave address + Write
    i2c_write(slaveAddr << 1);

    // EEPROM memory address (16-bit)
    i2c_write(wBuffAddr >> 8);
    i2c_write((u8)wBuffAddr);

    // Data byte
    i2c_write(dat);

    // Stop condition
    i2c_stop();

    // EEPROM internal write cycle delay
    delay_ms(10);
}


/*
 * Random read single byte from EEPROM
 */
u8 i2c_eeprom_randomread(u8 slaveAddr, u16 rBuffAddr)
{
    u8 dat;

    // Start condition
    i2c_start();

    // Slave address + Write
    i2c_write(slaveAddr << 1);

    // EEPROM memory address
    i2c_write(rBuffAddr >> 8);
    i2c_write((u8)rBuffAddr);

    // Repeated start
    i2c_restart();

    // Slave address + Read
    i2c_write((slaveAddr << 1) | 1);

    // Read data and send NACK
    dat = i2c_nack();

    // Stop condition
    i2c_stop();

    return dat;
}


/*
 * Page write to EEPROM
 */
void i2c_eeprom_pagewrite(u8 slaveAddr, u16 wBuffStartAddr,u8 *p, u8 nBytes)
{
    u8 i;

    if(nBytes == 0)
        return;


    // Start condition
    i2c_start();

    // Slave address + Write
    i2c_write(slaveAddr << 1);


    // EEPROM address
    i2c_write(wBuffStartAddr >> 8);
    i2c_write((u8)wBuffStartAddr);


    // Send data bytes
    for(i = 0; i < nBytes; i++)
    {
        i2c_write(p[i]);
    }


    // Stop condition
    i2c_stop();


    // EEPROM write cycle delay
    delay_ms(10);
}


/*
 * Sequential read from EEPROM
 */
void i2c_eeprom_seqread(u8 slaveAddr, u16 rBuffStartAddr, u8 *p, u8 nBytes)
{
    u8 i;


    if(nBytes == 0)
        return;


    // Start condition
    i2c_start();


    // Slave address + Write
    i2c_write(slaveAddr << 1);


    // EEPROM memory address
    i2c_write(rBuffStartAddr >> 8);
    i2c_write((u8)rBuffStartAddr);


    // Repeated start
    i2c_restart();


    // Slave address + Read
    i2c_write((slaveAddr << 1) | 1);


    // Read all bytes except last byte
    for(i = 0; i < (nBytes - 1); i++)
    {
        p[i] = i2c_mack();
    }


    // Last byte with NACK
    p[i] = i2c_nack();


    // Stop condition
    i2c_stop();
}
