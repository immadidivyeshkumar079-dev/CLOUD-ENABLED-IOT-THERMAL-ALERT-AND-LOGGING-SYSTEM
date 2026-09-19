//adc.c
#include "types.h"
#include<LPC21xx.h>
#include "adc_defines.h"
#include "delay.h"
#include "pin_connect_block.h"
#include "pin_function_defines.h"

void Init_ADC(void)
{
CfgPortPinFunc(0,28,PINFUNC2);
PINSEL1|=0X15400000;
//cfg ADCR
ADCR|=(1<<PDN_BIT)|(CLKDIV<<CLKDIV_BITS);
}
void Read_ADC(u32 chno,u32 *adcdval,f32 *adcar)
{
//clear any channel sel
ADCR&=0xffffff00;
//sel req channel & start conv
ADCR|=(1<<CONV_START_BIT)|(1<<chno);
//provide conv time/delay
delay_us(3);
//check until conversion success

while(((ADDR>>DONE_BIT)&1)==0);//stop conv
ADCR&=~(1<<CONV_START_BIT);
//read digital result
*adcdval=((ADDR>>RESULT_BITS)&1023);
*adcar=(*adcdval*(3.3/1024));//represent as voltage reading
}
