// STM32F407 Discovery Keil-freeRTOS Demonstration.
#include "stm32f4xx.h" // Device header
#include "FreeRTOS.h"  // ARM.FreeRTOS::RTOS:Core
#include "task.h"      // ARM.FreeRTOS::RTOS:Core

#include "leds.h"

// Delay time in ms.
#define DELAY_MS 1000

void redThread(void* arg1) 
{ 
  while(1) 
  { 
    RedToggle(); 
    vTaskDelay(DELAY_MS); 
  } 
	vTaskDelete(NULL);
}
void blueThread(void* arg1) { while(1) { BlueToggle(); vTaskDelay(DELAY_MS/2); } vTaskDelete(NULL); }
void greenThread(void* arg1) { while(1) { GreenToggle(); vTaskDelay(DELAY_MS/4); } vTaskDelete(NULL); }
void orangeThread(void* arg1) { while(1) { OrangeToggle(); vTaskDelay(DELAY_MS/8); } vTaskDelete(NULL); }

int main(void) 
{
	GPIO_Init();
	
  xTaskCreate(
	            blueThread,        /* task function */
	            "blue led toggle", /* description */
	            100,               /* stack size */
             	NULL,              /* parameters */
            	1,                 /* prioirity */
            	NULL               /* handle */
             );

  xTaskCreate(
	            redThread,         /* task function */
	            "red led toggle",  /* description */
	            100,               /* stack size */
             	NULL,              /* parameters */
            	1,                 /* prioirity */
            	NULL               /* handle */
             );

  xTaskCreate(
	            orangeThread,        /* task function */
	            "orange led toggle", /* description */
	            100,                 /* stack size */
             	NULL,                /* parameters */
            	1,                   /* prioirity */
            	NULL                 /* handle */
             );

  xTaskCreate(
	            greenThread,        /* task function */
	            "green led toggle", /* description */
	            100,                /* stack size */
             	NULL,               /* parameters */
            	1,                  /* prioirity */
            	NULL                /* handle */
             );

  vTaskStartScheduler();
	
	while(1);
}	

