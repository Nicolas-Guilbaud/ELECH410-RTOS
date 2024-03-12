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

// Task prototypes
void        adc_task();
void        lcd_task(); 
void        led_task(); 
void        event_sw1_task();

uint8 scheduler[6][3] = {
    {1,2,3},
    {1,0,0},
    {1,2,0},
    {1,0,3},
    {1,2,0},
    {1,0,0}
};

uint8 counter = 0;

uint16 adc_result = 0;

uint8 flags[] = {0,0,0};

int c = 0;

char8 msg[] = "Hello World !"; //size = 13
uint8 lcd_shift = 0;

CY_ISR(handler){
    
    
    
    
    counter = (counter+1) %6;
    
    
    for(uint8 i = 0; i < 3; i++){
        uint8 value = scheduler[counter][i];
        flags[value-1] = 1;
    }
    
    if(c) c--;
    else c++;
    
    GPIOJ12_Write(c);
    
    
    Timer_ReadStatusRegister();
}

void select_task(uint8 nber){
    switch(nber){
        case 1: 
            adc_task();
            break;
        default: 
            break;
    }
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    isr_timer_StartEx(handler);
    Timer_Start();
    LCD_Init();
    ADC_Init();
    
    for(;;)
    {
        /* Place your application code here. */
        for(uint8 i = 0; i < 3; i++){
            
            if(flags[i]){
                select_task(i);
                flags[i] = 0;
            }
            
        }
        
    }
}

void adc_task(){
    
    if(adc_result == 0)
        ADC_StartConvert();
    
    if(ADC_IsEndConversion(ADC_RETURN_STATUS)){
        
        adc_result = ADC_GetResult16();
        ADC_StartConvert();
    }
}

void led_task(){
}

uint8 lcd_counter = 0;
    
void lcd_task(){
    lcd_counter++;
    
    if(lcd_counter != 10)
        return;
    
    lcd_counter = 0;
    lcd_shift ++;
    
    LCD_ClearDisplay();
    LCD_Position(lcd_shift/8,lcd_shift%8);
    LCD_PrintString(msg);
    char msg2[13];
    for(int i = 0; i < 13; i ++){
        int index = (i+lcd_shift)%13;
        msg2[i] = msg[i];
    }
    
    
}

void event_sw1_task(){
}


/* [] END OF FILE */
