//kpm1.c
#include "types.h"
#include <LPC21xx.h>
#include "defines.h"
#include "delay.h"
#include "kpm_defines.h"
#include "lcd.h"
#include "lcd_defines.h"

// 4x4 Keypad lookup table
// Rows and columns are mapped to these characters
u8 kpmLUT[4][4] =
{
    {'1','2','3','4'},
    {'5','6','7','8'},
    {'9','*','0','#'},
    {'A','B','C','D'}
};

 // Initialize the keypad.
 // Configure row pins as outputs.
 // Configure column pins as inputs.

void Init_KPM(void)
{
    // Configure ROW0-ROW3 as GPIO outputs
    WRITENIBBLE(IODIR1, ROW0, 15);

    // Configure COL0-COL3 as GPIO inputs
    WRITENIBBLE(IODIR1, COL0, 0);
}


//Check whether any key is pressed.
// 0 -> Key Pressed
// 1 -> No Key Pressed

u32 ColScan(void)
{
    u32 status;

    // Read all column inputs
    status = READNIBBLE(IOPIN1, COL0);

    // If any column becomes LOW, a key is pressed
    return (status < 15) ? 0 : 1;
}

// Find the row in which the key is pressed.
// Row number (0-3)
u32 RowCheck(void)
{
    u32 r;

    // Scan each row one by one
    for(r = 0; r <= 3; r++)
    {
        // Make one row LOW at a time
        WRITENIBBLE(IOPIN1, ROW0, ~(1 << r));

        // Check if any column becomes LOW
        if(ColScan() == 0)
        {
            break;
        }
    }

    // Reset all rows LOW after scanning
    WRITENIBBLE(IOPIN1, ROW0, 0);

    return r;
}


 //ColCheck()
// Find the column in which the key is pressed.
// Column number (0-3)
u32 ColCheck(void)
{
    u32 c;

    // Check each column
    for(c = 0; c <= 3; c++)
    {
        if(READBIT(IOPIN1, COL0 + c) == 0)
            break;
    }

    return c;
}
// KeyScan()
// Detect which key is pressed.
// Character corresponding to the pressed key.
u8 KeyScan(void)
{
    u32 r, c;
    u8 keyV;

    // Wait until any key is pressed
    while(ColScan());

    // Find row number
    r = RowCheck();

    // Find column number
    c = ColCheck();

    // Read key value from lookup table
    keyV = kpmLUT[r][c];

    return keyV;
}
// Read a numeric value from keypad.
// Controls :
// 0-9 -> Enter digits
// A   -> Finish input
// B   -> Delete last digit
//Parameters :
// num     -> Stores final numeric value
// lastKey -> Stores last pressed key
void ReadNum(u32 *num, u32 *lastKey)
{
    u8 keyV;

    // Initialize number
    *num = 0;

    // Move LCD cursor to Line-2
    CmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        // Read key
        keyV = KeyScan();

        if(keyV == 0)
            continue;

        // Simple debounce delay
        delay_ms(200);

        // Store last key pressed
        *lastKey = keyV;

        // If digit is pressed
        if((keyV >= '0') && (keyV <= '9'))
        {
            // Convert ASCII digit to integer
            *num = (*num * 10) + (keyV - '0');

            // Display digit on LCD
            CharLCD(keyV);

            // Wait until key release
            while(ColScan() == 0);
        }

        // A key confirms the entered number
        else if(keyV == 'A')
        {
            while(ColScan() == 0);
            break;
        }

        // B key deletes previous digit
        else if(keyV == 'B')
        {
            if(*num > 0)
            {
                // Remove last digit
                *num = *num / 10;

                // Erase last character from LCD
                CmdLCD(SHIFT_DISP_LEFT);
                CharLCD(' ');
                CmdLCD(SHIFT_DISP_LEFT);
            }

            // Wait until key release
            while(ColScan() == 0);
        }
    }
}

// Read a string of numeric characters from keypad.

// Controls :
// 0-9 -> Store character
// A   -> Finish input
// B   -> Delete previous character
// Parameter :
//ptr -> Buffer to store entered string
void ReadValue(s8 *ptr)
{
    u32 i = 0;
    u8 keyV;

    // Move LCD cursor to Line-2
    CmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        // Read key
        keyV = KeyScan();

        if(keyV == 0)
            continue;

        // Debounce delay
        delay_ms(200);

        // If numeric key is pressed
        if((keyV >= '0') && (keyV <= '9'))
        {
            // Store character in buffer
            ptr[i++] = keyV;

            // Display character on LCD
            CharLCD(keyV);

            // Wait until key release
            while(ColScan() == 0);
        }

        // A key confirms the input
        else if(keyV == 'A')
        {
            while(ColScan() == 0);
            break;
        }

        // B key performs backspace
        else if(keyV == 'B')
        {
            if(i > 0)
            {
                // Move back one position
                i--;

                // Remove character from buffer
                ptr[i] = '\0';

                // Remove character from LCD
                CmdLCD(SHIFT_DISP_LEFT);
                CharLCD(' ');
                CmdLCD(SHIFT_DISP_LEFT);
            }

            // Wait until key release
            while(ColScan() == 0);
        }
    }

    // Append null character to make it a valid string
    ptr[i] = '\0';
}
