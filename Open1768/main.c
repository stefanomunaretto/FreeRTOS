/*
    FreeRTOS V9.0.0rc1 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.
*/



//##############################################################################
// INCLUDE FILES
//##############################################################################

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "lpc17xx.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_libcfg.h"   // define to set

#include "xtypes.h"
#include "led.h"
#include "debug_frmwrk.h"


// #############################################################################
// Data definition
// #############################################################################


// Constants definition
// =============================================================================

// queue lenght
#define mainQUEUE_LENGTH                (1)

// task param.
#define mainQUEUE_SEND_PARAMETER        (0x1111UL)
#define mainQUEUE_RCV_PARAMETER         (0x22UL)
#define mainLED_PARAM                   (0x3333UL)

// task priority
#define mainUIP_TASK_PRIORITY           (tskIDLE_PRIORITY + 4)
#define mainQUEUE_RCV_TASK_PRIORITY     (tskIDLE_PRIORITY + 3)
#define mainQUEUE_SEND_TASK_PRIORITY    (tskIDLE_PRIORITY + 2)
#define mainLED_TASK_PRIORITY           (tskIDLE_PRIORITY + 1)

// delay tick calc.
#define mainQUEUE_SEND_FREQUENCY_MS     (pdMS_TO_TICKS(1000UL))

#define mainLED_TOGGLE_RATE             ((TickType_t)100/portTICK_PERIOD_MS)


// The WEB server has a larger stack as it utilises stack hungry string handling library calls
#define mainBASIC_WEB_STACK_SIZE        (configMINIMAL_STACK_SIZE * 4)

// The message displayed by the WEB server when all tasks are executing without an error being reported
#define mainPASS_STATUS_MESSAGE         "All tasks are executing without error."



// Type data
// =============================================================================

// Private data
// =============================================================================

// queue used for blink
static QueueHandle_t xQueue = NULL;

// Holds the status message displayed by the WEB server.
static char *pcStatusMessage = mainPASS_STATUS_MESSAGE;


// Macros
// =============================================================================

// Prototype
// =============================================================================

// Configure the hardware for the demo.
static void prvSetupHardware( void );


// Simply returns the current status message for display on served WEB pages.
char *pcGetTaskStatusMessage( void );


//##############################################################################
// Private functions
//##############################################################################

static void prvQueueRcvTask(void* pvParameters)
{
   unsigned long ulReceivedValue;
   static const TickType_t xShortBlock = pdMS_TO_TICKS( 500 );

   /* Check the task parameter is as expected. */
   configASSERT( (unsigned long)pvParameters  == mainQUEUE_RECEIVE_PARAMETER );

   for( ;; ) {

      xQueueReceive(xQueue, &ulReceivedValue, portMAX_DELAY);

      if( ulReceivedValue == 100UL )
      {
         /*LED_SetValue(LED1,0);
         vTaskDelay( xShortBlock );
         //LED_SetValue(LED1,0);
         LED_Toggle(LED1);*/                     
         
         DBG_Printf("Welcome to use Open1768 development board\r\n");

         ulReceivedValue = 0U;
      }
   }
}


static void prvQueueSendTask(void* pvParameters)
{
   TickType_t xNextWakeTime;
   const unsigned long ulValueToSend = 100UL;

   /* Check the task parameter is as expected. */
   configASSERT(  (unsigned long)pvParameters  == mainQUEUE_SEND_PARAMETER );

   /* Initialise xNextWakeTime - this only needs to be done once. */
   xNextWakeTime = xTaskGetTickCount();

   for( ;; ) {

      vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

      xQueueSend( xQueue, &ulValueToSend, 0U );
   }
}


static void prvLedTask(void* pvParam)
{
   TickType_t xLastFlashTime;
   xLastFlashTime = xTaskGetTickCount();

   static const TickType_t xShortBlock = pdMS_TO_TICKS( 200 );
   static uint32 mask = 0x1;

   // Check the task parameter is as expected
   configASSERT((ulong)pvParameters == mainLED_PARAM);

   for( ;; ) {

      vTaskDelayUntil( &xLastFlashTime, mainLED_TOGGLE_RATE );
      //vTaskDelay( xShortBlock );

      LED_SetMaskBit(LEDS_PORT, mask, 0);
      mask = (mask << 1);
      if (mask == 0x00000004) mask = 0x1;
      LED_SetMaskBit(LEDS_PORT, mask, 1);
   }

}

// The task that handles the uIP stack.  All TCP/IP processing is performed in this task.
extern void vuIP_Task( void* pvParameters );


//##############################################################################
// Public functions
//##############################################################################

int main( void )
{
   BaseType_t bt = pdPASS;

   // configure hardware
   prvSetupHardware();

   // set led
   LED_Init();

   // uart0 dbg init
   debug_frmwrk_init();
   DBG_Printf("Welcome to use Open1768 development board\r\n");
   
   // emac init
   // see task vuIP_Task

   // create a queue
   xQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(uint32_t));

   if (xQueue != NULL) {

      // uart task
      bt = xTaskCreate(prvQueueRcvTask,  "Rx", configMINIMAL_STACK_SIZE, (void*)mainQUEUE_RCV_PARAMETER, mainQUEUE_RCV_TASK_PRIORITY, NULL);
      bt = xTaskCreate(prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, (void*)mainQUEUE_SEND_PARAMETER, mainQUEUE_SEND_TASK_PRIORITY, NULL);

      // led task
      bt = xTaskCreate(prvLedTask, "LedTask", configMINIMAL_STACK_SIZE, (void*)mainLED_PARAM, mainLED_TASK_PRIORITY, NULL);

      // http server task
      bt = xTaskCreate(vuIP_Task, "uIP", mainBASIC_WEB_STACK_SIZE, (void*) NULL, mainUIP_TASK_PRIORITY, NULL);
      
      vTaskStartScheduler();
   }

   // endless loop
   for( ;; );
}


void prvSetupHardware( void )
{
   #define PLLFEED_FEED1   0x000000AA
   #define PLLFEED_FEED2   0x00000055

   /* Disable peripherals power. */
   LPC_SC->PCONP = 0;

   /* Enable GPIO power. */
   LPC_SC->PCONP = CLKPWR_PCONP_PCGPIO;

   /* Disable TPIU. */
   LPC_PINCON->PINSEL10 = 0;

   if ( LPC_SC->PLL0STAT & ( 1 << 25 ) )
   {
       /* Enable PLL, disconnected. */
       LPC_SC->PLL0CON = 1;
       LPC_SC->PLL0FEED = PLLFEED_FEED1;
       LPC_SC->PLL0FEED = PLLFEED_FEED2;
   }

   /* Disable PLL, disconnected. */
   LPC_SC->PLL0CON = 0;
   LPC_SC->PLL0FEED = PLLFEED_FEED1;
   LPC_SC->PLL0FEED = PLLFEED_FEED2;

   /* Enable main OSC. */
   LPC_SC->SCS |= 0x20;
   while( !( LPC_SC->SCS & 0x40 ) );

   /* select main OSC, 12MHz, as the PLL clock source. */
   LPC_SC->CLKSRCSEL = 0x1;

   LPC_SC->PLL0CFG = 0x20031;
   LPC_SC->PLL0FEED = PLLFEED_FEED1;
   LPC_SC->PLL0FEED = PLLFEED_FEED2;

   /* Enable PLL, disconnected. */
   LPC_SC->PLL0CON = 1;
   LPC_SC->PLL0FEED = PLLFEED_FEED1;
   LPC_SC->PLL0FEED = PLLFEED_FEED2;

   /* Set clock divider. */
   LPC_SC->CCLKCFG = 0x03;

   /* Configure flash accelerator. */
   LPC_SC->FLASHCFG = 0x403a;

   /* Check lock bit status. */
   while( ( ( LPC_SC->PLL0STAT & ( 1 << 26 ) ) == 0 ) );

   /* Enable and connect. */
   LPC_SC->PLL0CON = 3;
   LPC_SC->PLL0FEED = PLLFEED_FEED1;
   LPC_SC->PLL0FEED = PLLFEED_FEED2;
   while( ( ( LPC_SC->PLL0STAT & ( 1 << 25 ) ) == 0 ) );




   /* Configure the clock for the USB. */

   if( LPC_SC->PLL1STAT & ( 1 << 9 ) )
   {
       /* Enable PLL, disconnected. */
       LPC_SC->PLL1CON = 1;
       LPC_SC->PLL1FEED = PLLFEED_FEED1;
       LPC_SC->PLL1FEED = PLLFEED_FEED2;
   }

   /* Disable PLL, disconnected. */
   LPC_SC->PLL1CON = 0;
   LPC_SC->PLL1FEED = PLLFEED_FEED1;
   LPC_SC->PLL1FEED = PLLFEED_FEED2;

   LPC_SC->PLL1CFG = 0x23;
   LPC_SC->PLL1FEED = PLLFEED_FEED1;
   LPC_SC->PLL1FEED = PLLFEED_FEED2;

   /* Enable PLL, disconnected. */
   LPC_SC->PLL1CON = 1;
   LPC_SC->PLL1FEED = PLLFEED_FEED1;
   LPC_SC->PLL1FEED = PLLFEED_FEED2;
   while( ( ( LPC_SC->PLL1STAT & ( 1 << 10 ) ) == 0 ) );

   /* Enable and connect. */
   LPC_SC->PLL1CON = 3;
   LPC_SC->PLL1FEED = PLLFEED_FEED1;
   LPC_SC->PLL1FEED = PLLFEED_FEED2;
   while( ( ( LPC_SC->PLL1STAT & ( 1 << 9 ) ) == 0 ) );

   /*  Setup the peripheral bus to be the same as the CPU output (100 MHz). */
   LPC_SC->PCLKSEL0 = 0x05555555;

}


void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
   /* This function will get called if a task overflows its stack. */

   ( void ) pxTask;
   ( void ) pcTaskName;

   for( ;; );
}

void vConfigureTimerForRunTimeStats( void )
{
   const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

   /* This function configures a timer that is used as the time base when
   collecting run time statistical information - basically the percentage
   of CPU time that each task is utilising.  It is called automatically when
   the scheduler is started (assuming configGENERATE_RUN_TIME_STATS is set
   to 1). */

   /* Power up and feed the timer. */
   LPC_SC->PCONP |= 0x02UL;
   LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3<<2))) | (0x01 << 2);

   /* Reset Timer 0 */
   LPC_TIM0->TCR = TCR_COUNT_RESET;

   /* Just count up. */
   LPC_TIM0->CTCR = CTCR_CTM_TIMER;

   /* Prescale to a frequency that is good enough to get a decent resolution,
   but not too fast so as to overflow all the time. */
   LPC_TIM0->PR =  ( configCPU_CLOCK_HZ / 10000UL ) - 1UL;

   /* Start the counter. */
   LPC_TIM0->TCR = TCR_COUNT_ENABLE;
}


void vApplicationTickHook( void )
{
//   static unsigned long ulTicksSinceLastDisplay = 0;
//
//   /* Called from every tick interrupt as described in the comments at the top
//   of this file.
//
//   Have enough ticks passed to make it	time to perform our health status
//   check again? */
//   ulTicksSinceLastDisplay++;
//   if( ulTicksSinceLastDisplay >= mainCHECK_DELAY )
//   {
//       /* Reset the counter so these checks run again in mainCHECK_DELAY
//       ticks time. */
//       ulTicksSinceLastDisplay = 0;
//
//       /* Has an error been found in any task? */
//       if( xAreGenericQueueTasksStillRunning() != pdTRUE )
//       {
//               pcStatusMessage = "An error has been detected in the Generic Queue test/demo.";
//       }
//       else if( xAreQueuePeekTasksStillRunning() != pdTRUE )
//       {
//               pcStatusMessage = "An error has been detected in the Peek Queue test/demo.";
//       }
//       else if( xAreBlockingQueuesStillRunning() != pdTRUE )
//       {
//               pcStatusMessage = "An error has been detected in the Block Queue test/demo.";
//       }
//       else if( xAreBlockTimeTestTasksStillRunning() != pdTRUE )
//       {
//               pcStatusMessage = "An error has been detected in the Block Time test/demo.";
//       }
//   else if( xAreSemaphoreTasksStillRunning() != pdTRUE )
//   {
//       pcStatusMessage = "An error has been detected in the Semaphore test/demo.";
//   }
//   else if( xArePollingQueuesStillRunning() != pdTRUE )
//   {
//       pcStatusMessage = "An error has been detected in the Poll Queue test/demo.";
//   }
//   else if( xAreIntegerMathsTaskStillRunning() != pdTRUE )
//   {
//       pcStatusMessage = "An error has been detected in the Int Math test/demo.";
//   }
//   else if( xAreRecursiveMutexTasksStillRunning() != pdTRUE )
//   {
//       pcStatusMessage = "An error has been detected in the Mutex test/demo.";
//   }
//   }
}


char *pcGetTaskStatusMessage( void )
{
   /* Not bothered about a critical section here. */
   return pcStatusMessage;
}