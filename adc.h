//adc.h
#include "types.h"
void Init_ADC(void);
void Read_ADC(u32 chno,u32 *adcdval,f32 *adcar);
u8 ADC_SensorConnected(u32 chno);
