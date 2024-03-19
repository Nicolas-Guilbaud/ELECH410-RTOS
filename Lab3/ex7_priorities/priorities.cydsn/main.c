/* ========================================
 *
 *              ELECH410 labs
 *          FreeRTOS example code
 * 
 *                   2019
 *
 * ========================================
*/
#include "project.h"

/* RTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

#define TASK_STACK_SIZE      (1024)

/* Task definitions */
#define TASK1_NAME           ("Task 1")
#define TASK2_NAME           ("Task 2")

//Will execute the task with higher priority first
#define TASK1_PRIORITY       (4)
#define TASK2_PRIORITY       (3)

#define TASK1_PERIOD         (1)
#define TASK2_PERIOD         (100)

/* Task prototypes */
void task1( void * );
void task2( void * );

/* Task handlers */
TaskHandle_t task1Handler; 
TaskHandle_t task2Handler; 

/* Function prototypes */
/*
 * Installs the RTOS interrupt handlers.
 */
static void freeRTOSInit( void );


int main(void)
{
    /* Enable global interrupts. */
    //CyGlobalIntEnable; 
    
    freeRTOSInit();
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    // ...
    
    // Create tasks
    xTaskCreate( task1, TASK1_NAME, TASK_STACK_SIZE, NULL, TASK1_PRIORITY, &task1Handler );
    xTaskCreate( task2, TASK2_NAME, TASK_STACK_SIZE, NULL, TASK2_PRIORITY, &task2Handler );
    
    // Launch freeRTOS
    vTaskStartScheduler();     
    
    for(;;)
    {
    }
}

void freeRTOSInit( void )
{
/* Port layer functions that need to be copied into the vector table. */
extern void xPortPendSVHandler( void );
extern void xPortSysTickHandler( void );
extern void vPortSVCHandler( void );
extern cyisraddress CyRamVectors[];

	/* Install the OS Interrupt Handlers. */
	CyRamVectors[ 11 ] = ( cyisraddress ) vPortSVCHandler;
	CyRamVectors[ 14 ] = ( cyisraddress ) xPortPendSVHandler;
	CyRamVectors[ 15 ] = ( cyisraddress ) xPortSysTickHandler;
}

void task1( void *arg )
{
    (void)arg;
    
    for(;;)
    {
        GPIOJ11_Write(1u);
        
        for (int i=0; i<10;i++){
            GPIOJ11_Write(!GPIOJ11_Read());
        }
        
        GPIOJ11_Write(0);
        
        vTaskDelay(TASK1_PERIOD);
    }

}

void task2( void *arg )
{
    (void)arg;
    
    for(;;)
    {
        GPIOJ12_Write(1u);
        
        for (int i=0; i<20000;i++){
            GPIOJ12_Write(!GPIOJ12_Read());
        }
        
        GPIOJ12_Write(0);
        
        vTaskDelay(TASK2_PERIOD);
    }
}


/* [] END OF FILE */
