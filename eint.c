/*#include<LPC21xx.h>
#include<stdlib.h>
#include "eint0.h"
#include "pin_connect_block.h"
#include "pin_function_defines.h"
#include "eint0_defines.h"
#include "types.h"
#include "lcd.h"
#include "kpm.h"
#include "esp01.h"
#include "i2c_eeprom.h"
#include "delay.h"

extern u32 flag;
extern s8 setPt[6];
extern u32 setTemp;
void Init_eint1(void)
{
        //cfg p0.3 pin as EINT0 input pin
        CfgPortPinFunc(0,14,PINFUNC3);

        //cfg VIC peripheral
        //cfg EINT0 as irq types,default all are irq type
        VICIntSelect = 0;
        //enable EINT0 via VIC
    VICIntEnable|=1<<EINT1_VIC_CHNO;
  //cfg  EINT0 as v.irq with highest priority
        //& allow to load eint0_isr addr
        VICVectCntl1 = (1<<5)|EINT1_VIC_CHNO;
        //load eint0 isr addr
        VICVectAddr1 =(u32)eint1_isr;

        //cfg External Interrupt Peripheral
        //allow/enable EINT0,default all are enabled
        //EXTINT = 1<<2;
        //cfg EINT0 as edge triggered
        EXTMODE = 1<<1;
        //cfg EINT0 as falling edge triggerd,
        //def all are falling edge
        EXTPOLAR = 0;

        //cfg EINT0 status LED pin as gpio out
}
void eint1_isr(void) __irq
{
        flag=1;
  //clear EINT0 status in External Interrupt Peripheral
        EXTINT = 1<<1;
        //clear EINT0 status in VIC
        VICVectAddr=0;
}
void Init_eint2(void)
{
        //cfg p0.3 pin as EINT0 input pin
        CfgPortPinFunc(0,15,PINFUNC3);

        //cfg VIC peripheral
        //cfg EINT0 as irq types,default all are irq type
        VICIntSelect = 0;
        //enable EINT0 via VIC
    VICIntEnable|=1<<EINT2_VIC_CHNO;
  //cfg  EINT0 as v.irq with highest priority
        //& allow to load eint0_isr addr
        VICVectCntl2 = (1<<5)|EINT2_VIC_CHNO;
        //load eint0 isr addr
        VICVectAddr2 =(u32)eint2_isr;

        //cfg External Interrupt Peripheral
        //allow/enable EINT0,default all are enabled
        //EXTINT = 1<<2;
        //cfg EINT0 as edge triggered
        EXTMODE = 1<<2;
        //cfg EINT0 as falling edge triggerd,
        //def all are falling edge
        EXTPOLAR = 0;

        //cfg EINT0 status LED pin as gpio out
}
void eint2_isr(void) __irq
{
        ClearLCD();
        StrLCD("   SET POINT");
        SetCursor(2,7);
        U32LCD(setTemp);
        delay_ms(2000);

  //clear EINT0 status in External Interrupt Peripheral
        EXTINT = 1<<2;
        //clear EINT0 status in VIC
        VICVectAddr=0;
        ClearLCD();
}
*/
#include <LPC21xx.h>

#include "types.h"
#include "eint0.h"
#include "eint0_defines.h"
#include "pin_connect_block.h"
#include "pin_function_defines.h"


/*=======================================================
                    EXTERNAL FLAG
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

extern volatile u32 flag;


/*=======================================================
                    ISR PROTOTYPES
=======================================================*/

void eint1_isr(void) __irq;
void eint2_isr(void) __irq;


/*=======================================================
                    INITIALIZE EINT1
=======================================================*/

/*
    P0.14 -> EINT1
    VIC channel = 15

    EINT1:
        Edge triggered
        Falling edge
        IRQ
*/

void Init_eint1(void)
{
    /* Configure P0.14 as EINT1 */
    CfgPortPinFunc(0, 14, PINFUNC3);

    /* Clear pending EINT1 interrupt */
    EXTINT = (1 << 1);

    /* EINT1 = edge triggered */
    EXTMODE |= (1 << 1);

    /* EINT1 = falling edge */
    EXTPOLAR &= ~(1 << 1);

    /* EINT1 = IRQ, not FIQ */
    VICIntSelect &= ~(1 << EINT1_VIC_CHNO);

    /* Configure VIC vector slot 1 */
    VICVectCntl1 = (1 << 5) | EINT1_VIC_CHNO;

    /* Load EINT1 ISR address */
    VICVectAddr1 = (u32)eint1_isr;

    /* Enable EINT1 interrupt */
    VICIntEnable |= (1 << EINT1_VIC_CHNO);
}


/*=======================================================
                    EINT1 ISR
=======================================================*/

void eint1_isr(void) __irq
{
    /* Tell main program that EINT1 occurred */
    flag = 1;

    /* Clear EINT1 interrupt */
    EXTINT = (1 << 1);

    /* Acknowledge VIC */
    VICVectAddr = 0;
}


/*=======================================================
                    INITIALIZE EINT2
=======================================================*/

/*
    P0.15 -> EINT2
    VIC channel = 16

    EINT2:
        Edge triggered
        Falling edge
        IRQ
*/

void Init_eint2(void)
{
    /* Configure P0.15 as EINT2 */
    CfgPortPinFunc(0, 15, PINFUNC3);

    /* Clear pending EINT2 interrupt */
    EXTINT = (1 << 2);

    /* EINT2 = edge triggered */
    EXTMODE |= (1 << 2);

    /* EINT2 = falling edge */
    EXTPOLAR &= ~(1 << 2);

    /* EINT2 = IRQ, not FIQ */
    VICIntSelect &= ~(1 << EINT2_VIC_CHNO);

    /* Configure VIC vector slot 2 */
    VICVectCntl2 = (1 << 5) | EINT2_VIC_CHNO;

    /* Load EINT2 ISR address */
    VICVectAddr2 = (u32)eint2_isr;

    /* Enable EINT2 interrupt */
    VICIntEnable |= (1 << EINT2_VIC_CHNO);
}


/*=======================================================
                    EINT2 ISR
=======================================================*/

void eint2_isr(void) __irq
{
    /* Tell main program that EINT2 occurred */
    flag = 2;

    /* Clear EINT2 interrupt */
    EXTINT = (1 << 2);

    /* Acknowledge VIC */
    VICVectAddr = 0;
}
