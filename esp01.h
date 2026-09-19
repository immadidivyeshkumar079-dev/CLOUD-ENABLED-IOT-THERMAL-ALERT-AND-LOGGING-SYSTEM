#ifndef _ESP01_H_
#define _ESP01_H_

#include "types.h"

extern char buff[200];
extern unsigned char i;

u8 ESP_Wait(char *str, u16 time);
void esp01_connectAP(void);
void esp01_sendToThingspeak(s8 *val);
void esp01_sendSPToThingspeak(s8 *val);
void esp01_readFromThingspeak(char *val);
void ESP_ClearBuffer(void);
#endif
