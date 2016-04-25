#ifndef _LED_H_
#define _LED_H_

#include "xtypes.h"

typedef struct _LED_STRUCT_ {

   uint8  port;
   uint32 pin;
} LED_STRUCT;


// LED1 (USED FOR UART0)
#define LED1_GPIO       GPIO0
#define LED1_PORT       (0)
#define LED1_PBIT       (BITVAL(3))
#define LED1_PDIR       LED1_GPIO->FIODIR
#define LED1_PSET       LED1_GPIO->FIOSET
#define LED1_PCLR       LED1_GPIO->FIOCLR
#define LED1_PGET       LED1_GPIO->FIOPIN

// LED2 (USED FOR UART0)
#define LED2_GPIO       GPIO0
#define LED2_PORT       (0)
#define LED2_PBIT       (BITVAL(2))
#define LED2_PDIR       LED2_GPIO->FIODIR
#define LED2_PSET       LED2_GPIO->FIOSET
#define LED2_PCLR       LED2_GPIO->FIOCLR
#define LED2_PGET       LED2_GPIO->FIOPIN

// LED3
#define LED3_GPIO       GPIO0
#define LED3_PORT       (0)
#define LED3_PBIT       (BITVAL(1))
#define LED3_PDIR       LED3_GPIO->FIODIR
#define LED3_PSET       LED3_GPIO->FIOSET
#define LED3_PCLR       LED3_GPIO->FIOCLR
#define LED3_PGET       LED3_GPIO->FIOPIN

// LED4
#define LED4_GPIO       GPIO0
#define LED4_PORT       (0)
#define LED4_PBIT       (BITVAL(0))
#define LED4_PDIR       LED4_GPIO->FIODIR
#define LED4_PSET       LED4_GPIO->FIOSET
#define LED4_PCLR       LED4_GPIO->FIOCLR
#define LED4_PGET       LED4_GPIO->FIOPIN


#define LEDS_PORT       (0)


// LED DEFINITIONS STRUCT
#define LED1            (LED_STRUCT){LED1_PORT,LED1_PBIT}
#define LED2            (LED_STRUCT){LED2_PORT,LED2_PBIT}
#define LED3            (LED_STRUCT){LED3_PORT,LED3_PBIT}
#define LED4            (LED_STRUCT){LED4_PORT,LED4_PBIT}



void LED_Init(void);
void LED_SetValue(LED_STRUCT led, bool val);
bool LED_GetValue(LED_STRUCT led);
void LED_Toggle(LED_STRUCT led);
void LED_SetMaskBit(uint8 port, uint32 mask, bool val);


#endif