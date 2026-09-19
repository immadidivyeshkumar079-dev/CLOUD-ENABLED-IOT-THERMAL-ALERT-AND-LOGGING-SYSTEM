//lcd.h
#include "types.h"
void writeLCD(u8 byte);
void CmdLCD(u8 cmdbyte);
void InitLCD(void);
void CharLCD(u8 asciival);
void StrLCD(s8*);
void U32LCD(u32);
void S32LCD(s32);
void F32LCD(f32,u8 ndp);
void buildCGRAM(u8 *,u8);
void ClearLCD(void);
void SetCursor(u8,u8);
void MsgScroll(s8 *,s32);
