/*#include "types.h"
#include "adc.h"

void Read_Temperature(s8 TempType,f32 *temp)
{
        u32 adcDVal;
        f32 adcAR,degC,degF,degK;
        Read_ADC(1,&adcDVal,&adcAR);
        degC=adcAR*100;
        degF=(degC*1.8)+32;
        degK=degC+273.15;
        if(TempType=='C')
                *temp=degC;
        else if(TempType=='F')
                *temp=degF;
        else if(TempType=='K')
                *temp=degK;
}*/
#include "types.h"
#include "adc.h"
#include "lm35.h"


/*=======================================================
                    READ LM35 TEMPERATURE
=======================================================*/

/*
    LM35 output:

        10 mV / degree C

    Therefore:

        Temperature(C) = Voltage / 0.01

        Temperature(C) = Voltage * 100
*/

void Read_Temperature(s8 TempType, f32 *temp)
{
    u32 adcDVal;
    f32 adcVoltage;
    f32 degC;


    /* Read LM35 connected to ADC channel 1 */
    Read_ADC(1, &adcDVal, &adcVoltage);


    /* Convert LM35 voltage to Celsius */
    degC = adcVoltage * 100.0;


    /* Select required temperature unit */

    if (TempType == 'C')
    {
        /* Celsius */
        *temp = degC;
    }
    else if (TempType == 'F')
    {
        /* Fahrenheit */
        *temp = (degC * 1.8) + 32.0;
    }
    else if (TempType == 'K')
    {
        /* Kelvin */
        *temp = degC + 273.15;
    }
    else
    {
        /* Invalid temperature type */
        *temp = 0.0;
    }
}

