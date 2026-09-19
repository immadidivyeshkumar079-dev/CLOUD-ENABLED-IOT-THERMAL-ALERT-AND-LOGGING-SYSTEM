#ifndef UART0_DEFINES_H

#define UART0_DEFINES_H


/* UART Pin Configuration Definitions */

#define PIN_FUN1 0

#define PIN_FUN2 1

#define PIN_FUN3 2

#define PIN_FUN4 3


#define TXD0_PIN_0_0 PIN_FUN2

#define RXD0_PIN_0_1 PIN_FUN2


/*

  UART0 Baud Rate Formula:

  -------------------------

  Baud Rate = PCLK / (16 * DIVISOR)

  where,

  DIVISOR = (U0DLM * 256) + U0DLL


  To get 9600 baud rate:

  - We calculate DIVISOR based on peripheral clock (PCLK)

*/


/* Clock and Baud Rate Definitions */

#define FOSC  12000000      // External crystal frequency = 12 MHz

#define CCLK  (FOSC*5)      // CPU Clock (CCLK) = 60 MHz (using PLL multiplier = 5)

#define PCLK  (CCLK/4)      // Peripheral Clock (PCLK) = 15 MHz (default setting)

#define BAUD  9600          // Desired baud rate

#define DIVISOR (PCLK/(16*BAUD))     // Divisor value for baud rate generation


/* U0LCR Register Bit Definitions */

#define _8BIT 3               // Define a symbolic constant "_8BIT" with value 3

                             // In UART Line Control Register (U0LCR), bits 1:0

                             // determine the word length:

                             //   00 -> 5-bit data

                             //   01 -> 6-bit data

                             //   10 -> 7-bit data

                             //   11 -> 8-bit data

                             // So, 3 (binary 11) selects 8-bit data mode.


#define WORD_LEN _8BIT       // Define "WORD_LEN" as alias for "_8BIT"

                             // This improves code readability and allows changing

                             // the data word length easily in one place if needed.


#define DLAB_BIT 7           // Bit 7 of U0LCR register enables access to Divisor Latch registers


/* U0LSR Register Bit Definitions */

#define RDR_BIT   0  // Bit 0 of U0LSR ? Receiver Data Ready

#define THRE_BIT  5  // Bit 5 of U0LSR ? Transmit Holding Register Empty

#define TEMT_BIT  6  // Bit 6 of U0LSR ? Transmitter Empty (shift register empty)




#endif
