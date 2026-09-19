//adc_defines.h
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define ADCCLK 3000000
#define CLKDIV ((PCLK/ADCCLK)-1)

#define CLKDIV_BITS 8   //bit 8 to 15
#define PDN_BIT 21
#define CONV_START_BIT 24

#define RESULT_BITS 6//bit 6 to 15
#define DONE_BIT 31
