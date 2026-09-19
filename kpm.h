//kpm.h

#include "types.h"

void Init_KPM(void);

u32  ColScan(void);

u32  RowCheck(void);

u32  ColCheck(void);

void ReadValue(s8 *);

u32  KeyScan(void);


void ReadNum(u32 *num,u32 *lastKey);
