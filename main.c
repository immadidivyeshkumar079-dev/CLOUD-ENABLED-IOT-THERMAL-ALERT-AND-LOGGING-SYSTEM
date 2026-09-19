#include <LPC21xx.H>
#include <stdlib.h>

#include "types.h"
#include "defines.h"
#include "delay.h"
#include "lcd.h"
#include "kpm.h"
#include "adc.h"
#include "lm35.h"
#include "rtc.h"
#include "uart0.h"
#include "eint0.h"
#include "myfunctions.h"
#include "esp01.h"
#include "pin_connect_block.h"
#include "i2c_eeprom.h"
#include "i2c_peripheral.h"


/*=======================================================
                    HARDWARE DEFINITIONS
=======================================================*/

#define LED                 5
#define BUZZER              4

#define EEPROM_SLAVE_ADDR   0x50


/*=======================================================
                EXTERNAL INTERRUPT FLAG
=======================================================*/

/*
    flag = 0  -> No event

    flag = 1  -> EINT1
                  P0.14
                  Change setpoint

    flag = 2  -> EINT2
                  P0.15
                  Display setpoint
*/

volatile u32 flag = 0;


/*=======================================================
                    SETPOINT VARIABLES
=======================================================*/

/*
    setPt:

        Maximum 5 characters + NULL

        Example:
            "025"
            "030"
            "100"

        Array size = 6
*/

s8 setPt[6];

u32 setTemp;
u32 newSetTemp;


/*=======================================================
                TEMPERATURE VARIABLES
=======================================================*/

f32 temp;

s8 *Lmvalue;


/*=======================================================
                    RTC VARIABLES
=======================================================*/

u32 last_min;


/*=======================================================
                    STARTUP MESSAGE
=======================================================*/

s8 msg[] = "CLOUD ENABLED IOT THERMAL SYS";


/*=======================================================
                        MAIN
=======================================================*/

int main(void)
{
    /*===================================================
                    HARDWARE INITIALIZATION
    ===================================================*/

    Init_I2C();

    InitLCD();

    Init_KPM();

    Init_ADC();

    Init_RTC();

    /*
        EINT1:
            P0.14 -> EINT1
            Change setpoint
    */
    Init_eint1();

    /*
        EINT2:
            P0.15 -> EINT2
            Display setpoint
    */
    Init_eint2();

    Init_UART0();


    /*===================================================
                    LED / BUZZER
    ===================================================*/

    /*
        P0.5 -> LED
        P0.4 -> BUZZER
    */

    IODIR0 |= (1 << LED);
    IODIR0 |= (1 << BUZZER);

    /*
        Initially OFF
    */
    IOCLR0 = (1 << LED);
    IOCLR0 = (1 << BUZZER);


    /*===================================================
                    STARTUP MESSAGE
    ===================================================*/

    MsgScroll(msg, sizeof(msg) - 1);


    /*===================================================
                    ESP8266 CONNECTION
    ===================================================*/

    esp01_connectAP();

    ClearLCD();


    /*===================================================
                READ SETPOINT FROM EEPROM
    ===================================================*/

    /*
        Read 6 bytes:

            5 characters
            +
            NULL character
    */

    i2c_eeprom_seqread(EEPROM_SLAVE_ADDR,0x0000,(u8 *)setPt,6);


    /*
        Always terminate string.

        This protects atoi() and StrLCD()
        from reading beyond the array.
    */

    setPt[5] = '\0';


    /*===================================================
                    EEPROM CHECK
    ===================================================*/

    /*
        EEPROM empty conditions:

            0xFF
            or
            NULL
    */

    if ((setPt[0] == (s8)0xFF) || (setPt[0] == '\0'))
    {
        /*-----------------------------------------------
                    EEPROM IS EMPTY
        ------------------------------------------------*/

        ClearLCD();

        StrLCD("ENTER SETPOINT:");

        SetCursor(2, 7);

        /*
            Read setpoint from keypad.
        */
        ReadValue(setPt);

        /*
            Ensure NULL termination.
        */
        setPt[5] = '\0';


        /*-----------------------------------------------
                    STORE IN EEPROM
        ------------------------------------------------*/

        i2c_eeprom_pagewrite(EEPROM_SLAVE_ADDR,0x0000,(u8 *)setPt,6);

        delay_s(1);
    }
    else
    {
        /*-----------------------------------------------
                    EXISTING SETPOINT
        ------------------------------------------------*/

        ClearLCD();

        StrLCD("OLD SET POINT:");

        SetCursor(2, 7);

        StrLCD(setPt);

        delay_s(1);
    }


    /*===================================================
                CONVERT SETPOINT TO INTEGER
    ===================================================*/

    setTemp = atoi((char *)setPt);


    /*
        Limit setpoint to the LM35 operating range
        used by this application.

        LM35 error condition in this project:
            below 0 C
            above 100 C
    */

    if (setTemp > 100)
    {
        setTemp = 100;

        /*
            Store corrected value as ASCII.
            This requires your existing conversion
            function if EEPROM must also be corrected.

            For now, controller uses 100 C.
        */
    }


    /*===================================================
                    STORE CURRENT MINUTE
    ===================================================*/

    /*
        Cloud update occurs when RTC minute changes.
    */

    last_min = MIN;


    ClearLCD();


    /*===================================================
                    MAIN LOOP
    ===================================================*/

    while (1)
    {
        /*================================================
                    EINT1
                    P0.14 -> EINT1
                    flag = 1

                    CHANGE SETPOINT
        =================================================*/

        if (flag == 1)
        {
            /*
                Clear event immediately.

                ISR can set the flag again later.
            */
            flag = 0;


            /*--------------------------------------------
                    INTERRUPT CONFIRMATION
            --------------------------------------------*/
            SetCursor(1, 2);
                        StrLCD("INT OCCUR");
                        delay_s(1);
                        ClearLCD();

            StrLCD("INT OK");

            delay_s(1);


            /*--------------------------------------------
                    GET NEW SETPOINT
            --------------------------------------------*/

            ClearLCD();

            StrLCD("CHANGE SETPOINT:");

            SetCursor(2, 7);

            ReadValue(setPt);

            /*
                Ensure NULL termination.
            */
            setPt[5] = '\0';


            /*--------------------------------------------
                    CONVERT NEW SETPOINT
            --------------------------------------------*/

            newSetTemp = atoi((char *)setPt);


            /*--------------------------------------------
                    CHECK SETPOINT RANGE
            --------------------------------------------*/

            if (newSetTemp > 100)
            {
                /*
                    Invalid setpoint.

                    Keep previous setpoint.
                */

                ClearLCD();

                StrLCD("INVALID SETPOINT");

                delay_s(2);

                ClearLCD();

                continue;
            }


            /*--------------------------------------------
                    COMPARE OLD AND NEW SETPOINT
            --------------------------------------------*/

            if (setTemp != newSetTemp)
            {
                /*
                    Upload new setpoint to ThingSpeak.
                */
                esp01_sendSPToThingspeak(setPt);


                /*
                    Store new setpoint in EEPROM.
                */
                i2c_eeprom_pagewrite(EEPROM_SLAVE_ADDR,0x0000,(u8 *)setPt,6);

                /*
                    Update current setpoint.
                */
                setTemp = newSetTemp;
            }


            ClearLCD();
        }


        /*================================================
                    EINT2
                    P0.15 -> EINT2
                    flag = 2

                    DISPLAY SETPOINT
        =================================================*/

        if (flag == 2)
        {
            /*
                Clear event immediately.
            */
            flag = 0;


            ClearLCD();

            StrLCD("   SET POINT");

            SetCursor(2, 7);

            U32LCD(setTemp);

            delay_ms(2000);

            ClearLCD();
        }


        /*================================================
                    READ LM35 TEMPERATURE
        =================================================*/

        /*
            LM35 is connected to ADC channel 1.

            Read_Temperature():

                ADC voltage
                    |
                    v
                LM35 conversion
                    |
                    v
                Celsius temperature
        */

        Read_Temperature('C', &temp);


        /*================================================
                    SENSOR ERROR CHECK
        =================================================*/

        /*
            Valid range for this project:

                0 C <= temperature <= 100 C
        */

        if ((temp < 0.0) || (temp > 100.0))
        {
            /*--------------------------------------------
                    SENSOR ERROR
            --------------------------------------------*/
                        //SetCursor(1, 2);
            ClearLCD();

            StrLCD("SENSOR ERROR");


            /*--------------------------------------------
                    BUZZER ON
            --------------------------------------------*/

            IOSET0 = (1 << BUZZER);

            delay_s(2);


            /*--------------------------------------------
                    BUZZER OFF
            --------------------------------------------*/

            IOCLR0 = (1 << BUZZER);

            continue;
        }


        /*================================================
                    TEMPERATURE CONTROL
        =================================================*/

        if (temp > (f32)setTemp)
        {
            /*================================================
                        HIGH TEMPERATURE
            =================================================*/

            /*
                Temperature > setpoint

                    LED    -> ON
                    BUZZER -> ON
            */

            IOSET0 = (1 << LED);

            IOSET0 = (1 << BUZZER);


            /*--------------------------------------------
                    DISPLAY TIME AND TEMPERATURE
            --------------------------------------------*/

            ClearLCD();

            SetCursor(1, 2);

            StrLCD("TIME    TEMP");

            SetCursor(2, 0);

            DisplayRTCTime(HOUR, MIN, SEC);

            SetCursor(2, 10);

            F32LCD(temp, 1);

            StrLCD("C");

            delay_s(2);


            /*--------------------------------------------
                    HIGH TEMPERATURE ALERT
            --------------------------------------------*/

            ClearLCD();

            SetCursor(1, 3);

            StrLCD("!! ALERT !!");

            SetCursor(2, 2);

            StrLCD("HIGH TEMP");

            delay_s(2);
        }
        else
        {
            /*================================================
                        NORMAL TEMPERATURE
            =================================================*/

            /*
                Temperature <= setpoint

                    LED    -> OFF
                    BUZZER -> OFF
            */

            IOCLR0 = (1 << LED);

            IOCLR0 = (1 << BUZZER);


            /*--------------------------------------------
                    DISPLAY TIME AND TEMPERATURE
            --------------------------------------------*/

            ClearLCD();

            SetCursor(1, 2);

            StrLCD("TIME    TEMP");

            SetCursor(2, 0);

            DisplayRTCTime(HOUR, MIN, SEC);

            SetCursor(2, 10);

            F32LCD(temp, 1);

            StrLCD("C");

            delay_ms(500);
        }


        /*================================================
                    CLOUD UPDATE
                    ONCE PER MINUTE

                    ThingSpeak:

                    Field 1 -> Setpoint
                    Field 2 -> Temperature
        =================================================*/

        if (MIN != last_min)
        {
            /*
                Store current minute.

                This prevents repeated updates
                during the same minute.
            */

            last_min = MIN;


            /*--------------------------------------------
                    CONVERT TEMPERATURE TO STRING
            --------------------------------------------*/

            Lmvalue = floatToStr(temp);


            /*--------------------------------------------
                    SEND TEMPERATURE
                    ThingSpeak Field 2
            --------------------------------------------*/

            esp01_sendToThingspeak(Lmvalue);


            /*--------------------------------------------
                    WAIT FOR THINGSPEAK
            --------------------------------------------*/

            /*
                ThingSpeak minimum update interval
                is handled by this delay in the
                existing project design.
            */

            delay_ms(16000);


            /*--------------------------------------------
                    READ CLOUD SETPOINT
                    ThingSpeak Field 1
            --------------------------------------------*/

            esp01_readFromThingspeak(setPt);


            /*
                Ensure NULL termination.
            */

            setPt[5] = '\0';


            /*--------------------------------------------
                    VALIDATE CLOUD SETPOINT
            --------------------------------------------*/

            newSetTemp = atoi((char *)setPt);

            if (newSetTemp <= 100)
            {
                /*----------------------------------------
                        STORE CLOUD SETPOINT
                ----------------------------------------*/

                i2c_eeprom_pagewrite(EEPROM_SLAVE_ADDR,0x0000,(u8 *)setPt,6);

                delay_ms(500);


                /*----------------------------------------
                        READ BACK FROM EEPROM
                ----------------------------------------*/

                i2c_eeprom_seqread(EEPROM_SLAVE_ADDR,0x0000,(u8 *)setPt, 6);
                /*
                    Ensure NULL termination.
                */

                setPt[5] = '\0';


                /*----------------------------------------
                        UPDATE CONTROLLER SETPOINT
                ----------------------------------------*/

                setTemp = atoi((char *)setPt);
            }
        }
    }
}
