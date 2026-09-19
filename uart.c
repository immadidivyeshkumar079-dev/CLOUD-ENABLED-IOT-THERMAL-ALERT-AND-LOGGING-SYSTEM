#include <LPC21xx.H>

#include "rtc_defines.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"



// Array to hold names of days of the week
char week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};


/*
Initialize the Real-Time Clock (RTC)
This function disables the RTC, sets the prescaler values,
and then enables the RTC.
*/
void Init_RTC(void)
{
  // Disable and reset the RTC
        CCR = RTC_RESET;

#ifdef _LPC2148
  // Enable the RTC & select the clock source
        CCR = RTC_ENABLE | RTC_CLKSRC;
#else
  // Set prescaler integer and fractional parts
        PREINT = PREINT_VAL;
        PREFRAC= PREFRAC_VAL;

  // Enable the RTC
        CCR = RTC_ENABLE;

#endif

}

/*
Get the current RTC time
hour Pointer to store the current hour
minute Pointer to store the current minute
second Pointer to store the current second
*/
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
        *hour = HOUR;
        *minute = MIN;
        *second = SEC;
}

/*
Display the RTC time on LCD
hour value (0 23)
minute value (0 59)
second value (0 59) seperated by ':'
*/
void DisplayRTCTime(u32 hour, u32 minute, u32 second)
{
                //CmdLCD(GOTO_LINE1_POS0);
                CharLCD((hour/10)+48);
                CharLCD((hour%10)+48);
                CharLCD(':');
                CharLCD((minute/10)+48);
                CharLCD((minute%10)+48);
                CharLCD(':');
                CharLCD((second/10)+48);
                CharLCD((second%10)+48);

}

/*
Get the current RTC date
day Pointer to store the current date (1 31)
month Pointer to store the current month (1 12)
year Pointer to store the current year (four digits)
*/
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year)
{
        *date = DOM;
        *month = MONTH;
        *year = YEAR;
}

/*
Display the RTC date on LCD
Day of month (1 31)
Month (1 12)
Year (four digits) and seperated by '/'
*/
void DisplayRTCDate(u32 date, u32 month, u32 year)
{
                CmdLCD(GOTO_LINE2_POS0);
                CharLCD((date/10)+48);
                CharLCD((date%10)+48);
                CharLCD('/');
                CharLCD((month/10)+48);
                CharLCD((month%10)+48);
                CharLCD('/');
                U32LCD(year);
}

/*
Set the RTC time
Hour to set (0 23)
Minute to set (0 59)
Second to set (0 59)
*/
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second)
{
        HOUR = hour;
        MIN = minute;
        SEC = second;
}

/*
Set the RTC date
day of month to set (1 31)
month to set (1 12)
year to set (four digits)
*/
void SetRTCDateInfo(u32 date, u32 month, u32 year)
{
        DOM = date;
        MONTH = month;
        YEAR = year;
}

/*
Get the current day of the week
dow Pointer to store Day of Week (0=Sunday, ..., 6=Saturday)
*/
void GetRTCDay(s32 *day)
{
        *day = DOW;
}

/*
Display the current day of the week on LCD
dow (Day of Week) (0=Sunday, ..., 6=Saturday)
*/
void DisplayRTCDay(u32 dow)
{
        CmdLCD(GOTO_LINE1_POS0+10);
        StrLCD(week[dow]);
}

/*
Set the day of the week in RTC
Day of Week to set (0=Sunday, ..., 6=Saturday)
*/
void SetRTCDay(u32 day)
{
        DOW = day;
}
v25he1i2@srvr2:~/cloud majarprojest$ cat uart.c
#include <LPC21xx.H>


#include "uart_defines.h"

#include "pin_connect_block.h"

#define UART_INT_ENABLE 1

char buff[200],dummy;

unsigned char i=0,ch;


void UART0_isr(void) __irq

{

  if((U0IIR & 0x04)) //check if receive interrupt

  {

                ch = U0RBR;     /* Read to Clear Receive Interrupt */

                if(i<199){

                        buff[i++] = ch;
                        buff[i] = '\0';
                }
  }

  else
  {

      dummy=U0IIR; //Read to Clear transmit interrupt
  }

   VICVectAddr= 0; /* dummy write */

}
/*

Function: Init_UART0()

Purpose : Initialize UART0 peripheral for serial communication

*/

void Init_UART0(void)
{

        // Configure P0.0 as TXD0 and P0.1 as RXD0 (alternate function selection)

        //CfgPortPinFunc(0,0,TXD0_PIN_0_0);

        //CfgPortPinFunc(0,1,RXD0_PIN_0_1);
        PINSEL0|=0x00000005;

        // Configure UART0 line control for:

        // - 8 data bits

        // - 1 stop bit

        // - No parity

        // - DLAB = 1 (to access baud rate divisor registers)

        U0LCR = WORD_LEN| (1<<DLAB_BIT);

        // Load divisor value for 9600 baud rate

        // Lower 8 bits of divisor

        // Upper 8 bits of divisor

        U0DLL = DIVISOR;

        U0DLM = DIVISOR>>8;


        // Disable DLAB (so that THR and RBR can be accessed)

        U0LCR &=~(1<<DLAB_BIT);

#if UART_INT_ENABLE > 0

  VICIntSelect = 0x00000000; // IRQ

  VICIntEnable|= 1 << 6;   /* Enable UART0 Interrupt */

  VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */

  VICVectAddr0 = (unsigned)UART0_isr;
 // U0IIR = 0xc0;

  U0FCR = 0xc7;

  U0IER =1<<0;       /* Enable UART0 RX and THRE Interrupts */

  #endif

}


/*

Function: UART0_Txchar()

Purpose : Transmit one byte of data using UART0

*/

void UART0_TxChar(unsigned char TxByte)

{

        // Load the data byte into UART0 Transmit Holding Register

        U0THR = TxByte;

        // Wait until Transmitter is completely empty (TEMT = 1 ? both THR and shift register are empty)

        while(((U0LSR>>TEMT_BIT)&1) == 0);

}


/*

Function: UART0_RxChar()

Purpose : Receive one byte of data from UART0

*/

unsigned char UART0_RxChar(void)

{

        // Wait until Receiver Data Ready (RDR = 1)

        while(((U0LSR>>RDR_BIT)&1) == 0);

        // Return the received data byte from Receiver Buffer Register

        return U0RBR;

}




/*

Function: UART0_TxString()

Purpose : Transmit one string using UART0

*/

void UART0_TxStr(char *str)

{

        while(*str)

                UART0_TxChar(*str++);


}


/*

Function: UART0_TxUInt()

Purpose : Transmit one +ve integer value using UART0

*/

//add comments for each line of ts function

void UART0_TxUInt(u32 n)

{

        s32 i=0;

        u8 a[10];



        if(n==0)

        {

                UART0_TxChar('0');

        }

        else

        {

                while(n>0)

                {

                  a[i++]=(n%10)+48;

      n/=10;

                }

                for(--i;i>=0;i--)

                  UART0_TxChar(a[i]);

        }

}


/*

Function: UART0_TxSInt()

Purpose : Transmit one -ve integer value using UART0

*/

//add comments for each line of this function

void UART0_TxSInt(s32 n)

{

  if(n<0)

  {

                UART0_TxChar('-');

                n=-n;

        }

        UART0_TxUInt(n);

}


/*

Function: UART0_TxFloat()

Purpose : Transmit one float value using UART0

*/

//add comments for each line of this function

void UART0_TxFloat(f32 fn,u8 nDP)

{

        u32 n,i;

        if(fn<0.0)

        {

                UART0_TxChar('-');

                fn=-fn;

        }

  n=fn;

  UART0_TxUInt(n);

  UART0_TxChar('.');

  for(i=0;i<nDP;i++)

  {

                fn=(fn-n)*10;

                n=fn;

                UART0_TxChar(n+48);

  }

}




/*

Function: UART0_RxString()

Purpose : receive onne string through UART0

*/

//add comments for each line of this function

void UART0_RxString(u8 *str,u32 MaxLen)

{

        u32 i=0;

        u8 ch;



        while(1)

        {

                ch = UART0_RxChar();

                if(ch == '\r' || ch == '\n')

                {

                        str[i] = '\0';

                        break;

                }

                else if(i>=MaxLen-1)

                {

                        str[i] = '\0';

                        break;

                }

                else

                        str[i++] = ch;

        }

}
void UART0_Txhex(u32 n)
{
s32 i=0,nibble;
u8 a[8];
if(n==0)
UART0_TxChar('0');
else
{
while(n)
{
nibble=n%16;
nibble=(nibble>9)?((nibble-10)+'A'):(nibble+48);
a[i++]=nibble;
n/=16;
}
for(--i;i>=0;i--)
{
UART0_TxChar(a[i]);
}
}
}
void UART0_Txoct(u32 n)
{
s32 i=0,x;
u8 a[15];
if(n==0)
UART0_TxChar('0');
else
{
while(n)
{
x=n%8;
a[i++]=x+48;
n/=8;
}
for(--i;i>=0;i--)
{
UART0_TxChar(a[i]);
}
}
}