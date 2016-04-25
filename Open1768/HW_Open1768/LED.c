//##############################################################################
// Module Name  : LED.c
// Purpose      : led driver output for open1768
// Version      : 1.00
// Language     : C
// Author       : Stefano Munaretto
//##############################################################################


//##############################################################################
// INCLUDE FILES
//##############################################################################
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "lpc17xx_gpio.h"
#include "led.h"


// #############################################################################
// Data definition
// #############################################################################


// Constants definition
// =============================================================================

// Type data
// =============================================================================

// Private data
// =============================================================================

// Macros
// =============================================================================
#define LED_L1_HI()     (LED1_PSET = LED1_PBIT)
#define LED_L1_LO()     (LED1_PCLR = LED1_PBIT)
#define LED_L1(b)       ((b) ? LED_L1_HI() : LED_L1_LO())


//##############################################################################
// Private functions
//##############################################################################

//##############################################################################
// Public functions
//##############################################################################


void LED_SetValue(LED_STRUCT led, bool val)
{
   // neg. logic
   if (val)
      GPIO_ClearValue(led.port, led.pin);
   else
      GPIO_SetValue(led.port, led.pin);
}

bool LED_GetValue(LED_STRUCT led)
{
   return GPIO_ReadValue(led.port)&led.pin;

}

void LED_Toggle(LED_STRUCT led)
{
   bool lsts = GPIO_ReadValue(led.port)&led.pin;

   LED_SetValue(led, lsts);
}

void LED_SetMaskBit(uint8 port, uint32 mask, bool val)
{
   if (val)
      GPIO_ClearValue(port, mask);
   else
      GPIO_SetValue(port, mask);
}

void LED_Init()
{
   // init direction
   //GPIO_SetDir(LED1.port, LED1.pin, 1);   (USED FOR UART0)
   //GPIO_SetDir(LED2.port, LED2.pin, 1);   (USED FOR UART0)
   GPIO_SetDir(LED3.port, LED3.pin, 1);
   GPIO_SetDir(LED4.port, LED4.pin, 1);
   
   // all power off
   //LED_SetValue(LED1, 0);   (USED FOR UART0)
   //LED_SetValue(LED2, 0);   (USED FOR UART0)
   LED_SetValue(LED3, 0);
   LED_SetValue(LED4, 0);
}

