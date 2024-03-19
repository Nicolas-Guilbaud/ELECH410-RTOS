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
#define TASK3_NAME           ("Task 3")

#define TASK1_PRIORITY       (3)
#define TASK2_PRIORITY       (4)
#define TASK3_PRIORITY       (5)

#define TASK1_PERIOD         (10)
#define TASK2_PERIOD         (50)
#define TASK3_PERIOD         (100)

//Macro that allows to preempt the task to respect the period
#define REFINE(val) val - (xTaskGetTickCount()%val)

/* Task prototypes */
void task1( void * );
void task2( void * );
void task3( void * );

/* Task handlers */
TaskHandle_t task1Handler; 
TaskHandle_t task2Handler; 
TaskHandle_t task3Handler;

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
    xTaskCreate( task3, TASK3_NAME, TASK_STACK_SIZE, NULL, TASK3_PRIORITY, &task3Handler );
    
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

        for (int i=0; i<1000;i++){
            GPIOJ11_Write(!GPIOJ11_Read());
        }

        GPIOJ11_Write(0);

        vTaskDelay(REFINE(TASK1_PERIOD));
    }
}

void task2( void *arg )
{
    (void)arg;
    
    for(;;)
    {
        GPIOJ12_Write(1u);
        
        for (int i=0; i<2000;i++){
            GPIOJ12_Write(!GPIOJ12_Read());
        }
        
        GPIOJ12_Write(0);
        
        vTaskDelay(REFINE(TASK2_PERIOD));
    }
}

void task3( void *arg )
{
    (void)arg;
    
    for(;;)
    {
        GPIOJ13_Write(1u);
        
        for (int i=0; i<5000;i++){
            GPIOJ13_Write(!GPIOJ13_Read());
        }

        GPIOJ13_Write(0);

        vTaskDelay(REFINE(TASK3_PERIOD));
    }
}

/* [] END OF FILE */
