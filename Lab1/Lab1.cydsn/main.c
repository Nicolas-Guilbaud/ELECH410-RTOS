/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

void exo1_loop(){
    uint8 s1 = SW1_Read();
    LED1_Write(s1);
    
    uint8 s2 = SW2_Read();
    LED2_Write(s2);
    
    uint8 s3 = SW3_Read();
    LED3_Write(s3);
    
    uint8 s4 = SW4_Read();
    LED4_Write(s4);
    
    //CyDelay(50);
    
}

void exo2_setup(){
    LCD_Init();
}

uint8 SW_counter = 0;

void exo2_loop(){
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("SW2 coun");
    LCD_Position(1,0);
    LCD_PrintString("t: ");

    
    LCD_PrintDecUint16(SW_counter);
    
    while(!SW1_Read());
    SW_counter++;
    while(SW1_Read());
    
    CyDelay(50);
    
}

void exo3_init(){
    LCD_Init();
    keyboard_Start();
}

void exo3_loop(){
    LCD_ClearDisplay();
    
    char8 letter = keyboard_Scan();
    if(letter != 'z'){
        LCD_PutChar(letter);
    }
    CyDelay(50);
}

uint8 val = 0;

void exo4(){
    if(val)
        val--;
    else
        val++;
    
    GPIOJ11_Write(val);
    CyDelay(25);
}

void exo5(){
    GPIOJ12_Write(SW4_Read());   
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    for(;;)
    {
        /* Place your application code here. */
        
        exo4();
        exo5();
        
    }
}


/* [] END OF FILE */
