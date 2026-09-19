#include "types.h"
#include <LPC21xx.h>
#include "adc.h"
#include "lcd.h"
#include "delay.h"

#define BUZZER 4

s8 str[10];

s8* floatToStr(f32 fvalue)
{
    s8 tem[10];
    s32 num,val,digit,i=0,j=0;

    num = fvalue;
    val = num;

    if(num == 0)
    {
        str[j++]='0';
    }
    else
    {
        while(num)
        {
            digit=num%10;
            tem[i++]=digit+48;
            num/=10;
        }

        for(--i;i>=0;i--,j++)
        {
            str[j]=tem[i];
        }
    }

    str[j++]='.';

    fvalue=fvalue-val;
    fvalue*=10;

    val=fvalue;

    str[j++]=val+48;

    str[j]='\0';

    return str;
}
/*void SensorConnectionCheck(void)
{
    if(ADC_SensorConnected(1)==0)
    {
        ClearLCD();
        StrLCD("SENSOR ALERT");
        SetCursor(2,0);
        StrLCD("NOT CONNECTED");

        IOSET0 = 1<<BUZZER;
        delay_s(1);
        IOCLR0 = 1<<BUZZER;
    }
}


void SensorHealthCheck(f32 temp)
{
    if(temp < 0 || temp > 100)
    {
        ClearLCD();
        StrLCD("SENSOR ERROR");

        IOSET0 = 1<<BUZZER;
        delay_s(2);
        IOCLR0 = 1<<BUZZER;
    }
} */


/*void SensorHealthCheck1(f32 temp)
{

    if(temp < 0 || temp > 100)
    {

        ClearLCD();

        StrLCD("SENSOR ERROR");


        IOSET0 = 1<<BUZZER;


        delay_s(2);


        IOCLR0 = 1<<BUZZER;

    }

} */
