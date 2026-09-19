#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uart0.h"
#include "delay.h"
#include "lcd.h"

extern char buff[200];
extern unsigned char i;
extern u32 setTemp;


void esp01_sendSPToThingspeak(s8 *val)
{
    char req[100];
    char cmd[25];

    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD("SENDING SETPOINT");
    CmdLCD(0xC0);
    StrLCD("TO CLOUD..");

    delay_ms(1000);

    UART0_TxStr("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

    i = 0;
    memset(buff,'\0',200);

    while(i < 5);

    delay_ms(2500);

    buff[i] = '\0';

    if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED"))
    {
        //HTTP GET Request Create
        sprintf(req,"GET /update?api_key=YEJNQWSBD28CDLMT&field1=%s\r\n\r\n",val);

        //Calculate Length Automatically
        sprintf(cmd,"AT+CIPSEND=%d\r\n",strlen(req));

        UART0_TxStr(cmd);

        i = 0;
        memset(buff,'\0',200);

        delay_ms(500);

        // Send HTTP Request
        UART0_TxStr(req);

        delay_ms(5000);
        delay_ms(5000);

        buff[i] = '\0';

        delay_ms(2000);

        if(strstr(buff,"SEND OK"))
        {
            CmdLCD(0x01);
            StrLCD("SETPOINT UPDATED");
            delay_ms(1000);
        }
        else
        {
            CmdLCD(0x01);
            StrLCD("SETPOINT NOT UPDATED");
            delay_ms(1000);
        }
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }
}



void esp01_readFromThingspeak(char *val)
{

        s8 *ptr;

        CmdLCD(0x01);

         CmdLCD(0x80);
         StrLCD("READING FROM");
         CmdLCD(0xc0);
         StrLCD("CLOUD...");
        //StrLCD("AT+CIPSTART");

        delay_ms(1000);

        UART0_TxStr("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

        i=0;memset(buff,'\0',200);

        //while(i<5);

        delay_ms(2500);

        buff[i] = '\0';

        //CmdLCD(0x01);

        //CmdLCD(0x80);

        //StrLCD(buff);

        //delay_ms(2000);

        if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED"))

        {

                //CmdLCD(0xC0);

                //StrLCD("OK");

                //delay_ms(1000);



                //CmdLCD(0x01);

                //CmdLCD(0x80);

                //StrLCD("AT+CIPSEND");

                //delay_ms(1000);

                UART0_TxStr("AT+CIPSEND=66\r\n");

                i=0;memset(buff,'\0',200);

                delay_ms(500);

                //need to change the thingspeak write API key accordind to your channel

                UART0_TxStr("GET /channels/3489663/fields/1/last.txt?api_key=K3CTCXDW25EFZALK\r\n");

                delay_ms(5000);

                delay_ms(5000);

                buff[i] = '\0';

                delay_ms(2000);

        if((ptr=strchr(buff,':'))!=NULL)
         {
                //while(i++<4)

                        ptr++;
                        i=0;
                while((((*ptr>='0' && *ptr<='9')||(*ptr=='.'))&&(i<5)))
                {

                        val[i]=*ptr;

                        ptr++;
                        i++;

                }

                 val[i]='\0';

                 delay_ms(2000);

                 CmdLCD(0x01);

        }
        else
        {
            CmdLCD(0x01);

            SetCursor(1,0);

            StrLCD("data not read");

            delay_ms(3000);

            CmdLCD(0x01);

        }
         }
         else
         {

                CmdLCD(0xC0);

                StrLCD("ERROR");

                delay_ms(1000);

                return;
        }
}
void esp01_connectAP()
{
        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("CONNECTING TO");
        CmdLCD(0xc0);
        StrLCD("WIFI...");
        //StrLCD("AT");
        delay_ms(1000);
        UART0_TxStr("AT\r\n");
        i=0;memset(buff,'\0',200);
        while(i<4);
        delay_ms(500);
        buff[i]='\0';
        //CmdLCD(0x01);
        //CmdLCD(0x80);
        //StrLCD(buff);
        delay_ms(1000);
        if(strstr(buff,"OK"))
        {
                //CmdLCD(0xC0);
                //StrLCD("OK");
                delay_ms(1000);
        }
        else
        {
                CmdLCD(0xC0);
                StrLCD("ERROR");
                delay_ms(1000);
                return;
        }
        UART0_TxStr("ATE0\r\n");
        i=0;memset(buff,'\0',200);
        while(i<4);
        delay_ms(500);
        buff[i] = '\0';
        if(strstr(buff,"OK"))
        {
                delay_ms(1000);
        }
        else
        {
                CmdLCD(0xC0);
                StrLCD("ERROR");
                delay_ms(1000);
                return;
        }
        UART0_TxStr("AT+CIPMUX=0\r\n");
        i=0;memset(buff,'\0',200);
        while(i<4);
        delay_ms(500);
        buff[i] = '\0';
        delay_ms(2000);
        if(strstr(buff,"OK"))
        {
                delay_ms(1000);
        }
        else
        {
                CmdLCD(0xC0);
                StrLCD("ERROR");
                delay_ms(1000);
                return;
        }
        UART0_TxStr("AT+CWQAP\r\n");
        i=0;memset(buff,'\0',200);
        while(i<4);
        delay_ms(1500);
        buff[i] = '\0';
        delay_ms(2000);
        if(strstr(buff,"OK"))
        {
                delay_ms(1000);
        }
        else
        {
                CmdLCD(0xC0);
                StrLCD("ERROR");
                delay_ms(1000);
                return;
         }
        UART0_TxStr("AT+CWJAP=\"vivo T3 Ultra\",\"vivot333\"\r\n");
        i=0;memset(buff,'\0',200);
        while(i<4);
        delay_ms(2500);
        buff[i] = '\0';
        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD(buff);
        delay_ms(2000);
        if(strstr(buff,"WIFI CONNECTED"))
        {
                //CmdLCD(0xC0);
                //StrLCD("OK");
                delay_ms(1000);
        }
        else
        {
                CmdLCD(0xC0);
                StrLCD("ERROR");
                delay_ms(1000);
                return;
        }

}

void esp01_sendToThingspeak(s8 *val)
{
    char cmd[40];
    int len;
    int temp;

    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD("SENDING DATA");
    CmdLCD(0xC0);
    StrLCD("TO CLOUD...");

    UART0_TxStr("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

    i = 0;
    memset(buff,'\0',200);

    delay_ms(2500);

    buff[i] = '\0';

    if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED"))
    {
        temp = atoi(val);

        if(temp >= setTemp)
        {
            len = strlen("GET /update?api_key=YEJNQWSBD28CDLMT&field2=")
                + strlen(val)
                + strlen("&field3=1&status=OVERHEAT\r\n\r\n");
        }
        else
        {
            len = strlen("GET /update?api_key=YEJNQWSBD28CDLMT&field2=")
                + strlen(val)
                + strlen("&field3=0&status=NORMAL\r\n\r\n");
        }

        sprintf(cmd,"AT+CIPSEND=%d\r\n",len);
        UART0_TxStr(cmd);

        delay_ms(1000);

        UART0_TxStr("GET /update?api_key=YEJNQWSBD28CDLMT&field2=");
        UART0_TxStr(val);

        if(temp >= setTemp)
        {
            UART0_TxStr("&field3=1&status=OVERHEAT\r\n\r\n");
        }
        else
        {
            UART0_TxStr("&field3=0&status=NORMAL\r\n\r\n");
        }

        i = 0;
        memset(buff,'\0',200);

        delay_ms(5000);

        buff[i] = '\0';

        if(strstr(buff,"SEND OK"))
        {
            CmdLCD(0x01);
            StrLCD("DATA UPDATED");
            delay_ms(100);
        }
        else
        {
            CmdLCD(0x01);
            StrLCD("DATA NOT UPDATED");
            delay_ms(1000);

            CmdLCD(0x01);
            StrLCD(buff);
            delay_ms(3000);
        }
    }
    else
    {
        CmdLCD(0x01);
        StrLCD("CONNECT ERROR");
        delay_ms(1000);

        CmdLCD(0x01);
        StrLCD(buff);
        delay_ms(3000);
    }
}
