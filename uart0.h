#ifndef UART0_H_
#define UART0_H_

#include "types.h"

/* Function Declarations */
void Init_UART0(void);        // Function prototype for UART0 initialization
void UART0_TxChar(u8); // Function prototype for transmitting one byte
unsigned char UART0_RxChar(void); // Function prototype for receiving one byte
void UART0_TxStr(char *); // Function prototype for transmitting one string
void UART0_TxUInt(u32 n);
void UART0_TxSInt(s32 n);
void UART0_TxFloat(f32 fn,u8 nDP);
void UART0_RxString(u8 *str,u32 MaxLen);

void UART0_isr(void) __irq;
#endif

