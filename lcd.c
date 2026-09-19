//lcd.c
#include "types.h"
#include "delay.h"
#include "lcd_defines.h"
#include "defines.h"
#include<LPC21xx.h>

void writeLCD(u8 byte)
{
#if LCD_MODE==8
IOCLR0=1<<LCD_RW;
WRITEBYTE(IOPIN0,LCD_DATA,byte);
IOSET0=1<<LCD_EN;
delay_us(1);
IOCLR0=1<<LCD_EN;
delay_ms(2);
#elif LCD_MODE==4
#endif
}
void CmdLCD(u8 cmdbyte)
{
IOCLR0=1<<LCD_RS;
writeLCD(cmdbyte);
}
void InitLCD(void)
{
IODIR0|=((0xff<<LCD_DATA)|(1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));
delay_ms(15);
CmdLCD(MODE_8BIT_1LINE);//cmdLCD(0x30);
delay_ms(4);
delay_us(100);
CmdLCD(MODE_8BIT_1LINE);
delay_us(100);
CmdLCD(MODE_8BIT_1LINE);
CmdLCD(MODE_8BIT_2LINE);
CmdLCD(DSP_ON_CUR_BLINK);
CmdLCD(CLEAR_LCD);
CmdLCD(SHIFT_CUR_RIGHT);
}
void CharLCD(u8 asciival)
{
IOSET0=1<<LCD_RS;
writeLCD(asciival);
}
void StrLCD(s8 *s)
{
while(*s)
CharLCD(*s++);
}
void U32LCD(u32 n)
{
s32 i=0;
u8 a[10];
if(n==0)
CharLCD('0');
else
{
while(n>0)
{
a[i++]=(n%10)+48;
n/=10;
}
for(--i;i>=0;i--)
CharLCD(a[i]);
}
}
void S32LCD(s32 n)
{
if(n<0)
{
CharLCD('-');
n=-n;
}
U32LCD(n);
}
void F32LCD(f32 fn,u8 ndp)
{
u32 n,i;
if(fn<0.0)
{
CharLCD('-');
fn=-fn;
}
n=fn;
U32LCD(n);
CharLCD('.');
for(i=0;i<ndp;i++)
{
fn=(fn-n)*10;
n=fn;
CharLCD(n+48);
}
}
void buildCGRAM(u8 *p,u8 nbytes)
{
u32 i;
CmdLCD(GOTO_CGRAM_START);
IOSET0=1<<LCD_RS;
for(i=0;i<nbytes;i++)
{
writeLCD(p[i]);
}
CmdLCD(GOTO_LINE1_POS0);
}

void ClearLCD(void)
{
CmdLCD(CLEAR_LCD);
CmdLCD(GOTO_LINE1_POS0);
}
void changemsg(void)

{

                CmdLCD(CLEAR_LCD);

                CmdLCD(GOTO_LINE1_POS0+4);

                StrLCD("CHANGED");

                CmdLCD(GOTO_LINE2_POS0+1);

                StrLCD("SUCCESSFULLY");

                delay_ms(800);

}
void updatemsg(void)

{

                CmdLCD(CLEAR_LCD);

                CmdLCD(GOTO_LINE1_POS0+4);

                StrLCD("UPDATED");

                CmdLCD(GOTO_LINE2_POS0+1);

                StrLCD("SUCCESSFULLY");

                delay_ms(600);

}

void  MsgScroll(s8 *s,s32 size)
{
        u32 i=0;
        while(*s)
        {
                if(i==13)
                {
                        CmdLCD(GOTO_LINE2_POS0);
                }
                CharLCD(*s);
                delay_ms(250);
                s++;
                i++;

        }
}
void SetCursor(u8 line,u8 pos)
{
        if(line==1)
                CmdLCD(GOTO_LINE1_POS0+pos);
        else if(line==2)
                CmdLCD(GOTO_LINE2_POS0+pos);
}
