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

#define READ(name) name##_##Read()
#define WRITE(name,v) name##_##Write(v)
#define ANALYZE(name) WRITE(name,(READ(name)+1)%2);


#define TASK_STACK_SIZE      (1024)

/* Task definitions */
#define BLINKY_NAME           ("Blinky")
#define BLINKY_PRIORITY       (3)
#define BLINKY_PERIOD         (1000) // ticks

/* Task prototypes */
void blinkyTask( void * );
void blinkyTask2(void *);

/* Task handlers */
TaskHandle_t blinkyHandler;

/* Function prototypes */
/*
 * Installs the RTOS interrupt handlers.
 */
static void freeRTOSInit( void );

int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable; 

    freeRTOSInit();
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    // ...
    
    // Create tasks
    xTaskCreate( blinkyTask, BLINKY_NAME, TASK_STACK_SIZE, NULL, BLINKY_PRIORITY, &blinkyHandler );
    xTaskCreate( blinkyTask2, ("Blinky2"), TASK_STACK_SIZE, NULL, BLINKY_PRIORITY, &blinkyHandler);
    
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

void blinkyTask( void *arg )
{
    (void)arg;
    
    for(;;)
    {
        vTaskDelay(BLINKY_PERIOD);
        
        ANALYZE(LED1)
        
        
    }
}

void blinkyTask2(void *arg){
    (void) arg;
    
    for(;;){
        vTaskDelay(BLINKY_PERIOD/2);
        
        ANALYZE(LED2);
    }
}

/* [] END OF FILE */
