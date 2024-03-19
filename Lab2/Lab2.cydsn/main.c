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

#define READ(name) name##_##Read()
#define WRITE(name,v) name##_##Write(v)
#define ANALYZE(name) WRITE(name,(READ(name)+1)%2);



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

char8 msg[] = "Hello World !"; //size = 13
uint8 lcd_shift = 0;

CY_ISR(handler){
    
    ANALYZE(GPIOJ11);
    
    counter = (counter+1) %6;
    
    for(uint8 i = 0; i < 3; i++){
        uint8 value = scheduler[counter][i];
        flags[value-1] = 1;
    }
    
    
    Timer_ReadStatusRegister();
}

void select_task(uint8 nber){
    switch(nber){
        case 0: 
            adc_task();
            led_task();
            break;
        case 1:
            lcd_task();
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
    ADC_Start();
    ADC_StartConvert();
    
    for(;;)
    {
        WRITE(GPIOJ12,1);
        /* Place your application code here. */
        for(uint8 i = 0; i < 3; i++){
            
            if(flags[i]){
                select_task(i);
                flags[i] = 0;
            }
            
        }
        
        if(SW1_ClearInterrupt())
            event_sw1_task();
        
        WRITE(GPIOJ12,0);
        
        CyPmAltAct(PM_SLEEP_TIME_NONE, PM_ALT_ACT_SRC_INTERRUPT);
        
    }
}

void adc_task(){
    
    
    
    if(ADC_IsEndConversion(ADC_RETURN_STATUS) == 0){
        return;
    }
    
    adc_result = ADC_GetResult16();
    ADC_StartConvert();
}

uint8 led_nber = 1;

void led_selector(uint8 led,uint8 value){
    switch(led){
        case 1:
        WRITE(LED1,value);
        break;
        case 2:
        WRITE(LED2,value);
        break;
        case 3:
        WRITE(LED3,value);
        break;
        case 4:
        WRITE(LED4,value);
        break;
        default:
        break;
    }
}

uint16 led_count = 0;

void led_task(){
    
    led_count++;
    
    uint16 count = (adc_result*100)/65536;
    
    if(led_count < count)
        return;
    
    ANALYZE(GPIOJ14);
    led_count = 0;
    
    led_selector(led_nber,0);
    led_nber++;
    
    if(led_nber > 4)
        led_nber = 1;
    
    led_selector(led_nber,1);
    
}

uint8 lcd_counter = 0;

void lcd_task(){

    lcd_counter++;
    
    if(lcd_counter < 10)
        return;

    ANALYZE(GPIOJ13)
    
    lcd_counter = 0;
    lcd_shift = lcd_shift + 1 %13;
    
    LCD_ClearDisplay();
    for(int i = 0; i < 13; i ++){
        int index = (i+lcd_shift)%16;
        
        /*
        if(i == 0 || index % 8 == 0)
            LCD_Position(index/8,index%8);
        */
        if(index %8 == 0 || i == 0)
            LCD_Position(index/8,index%8);
        LCD_PutChar(msg[i]);
    }
    
}

uint8 sw_count = 0;

void event_sw1_task(){
    
    sw_count++;
    
    LCD_ClearDisplay();
    LCD_Position(1,5);
    LCD_PrintDecUint16(sw_count);
}


/* [] END OF FILE */
