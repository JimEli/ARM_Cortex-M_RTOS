// STM32F407 Discovery Keil-micrium osII RTOS Demonstration.
#include "stm32f4xx.h" // Device header
#include "os.h"        // Micrium.Micrium::RTOS:uC/OS Kernel

#include "leds.h"

// Stack size.
#define STACK_SIZE 128u
// Delay time in ms.
#define DELAY_MS 1000

OS_ERR osError;
OS_TCB osRedThreadTcb;
CPU_STK redThreadStack[STACK_SIZE];

// STM32F407 Discovery
// AHB1 PORT D clock enable
#define PORTD_CLOCK (1<<3)
// Colored LEDs  
#define GREEN       (1U<<12) // D.12
#define ORANGE      (1U<<13) // D.13
#define RED         (1U<<14) // D.14
#define BLUE        (1U<<15) // D.15
#define GREEN_BIT   (1U<<24)
#define ORANGE_BIT  (1U<<26)
#define RED_BIT     (1U<<28)
#define BLUE_BIT    (1U<<30)

void GPIO_Init()
{
	// Enable clock access to PORT of PIN.
	RCC->AHB1ENR |= PORTD_CLOCK;
  // Set the PIN's mode.
  GPIOD->MODER |= (RED_BIT | GREEN_BIT | BLUE_BIT | ORANGE_BIT);	
}

void RedToggle(void) { GPIOD->ODR ^= RED; }
void BlueToggle(void) { GPIOD->ODR ^= BLUE; }
void GreenToggle(void) { GPIOD->ODR ^= GREEN; }
void OrangeToggle(void) { GPIOD->ODR ^= ORANGE; }

void redThread(void* arg1) 
{ 
  while(DEF_TRUE)
  {
    RedToggle();
//		OSTimeDly(DELAY_MS, OS_OPT_TIME_DLY, &osError);
  } 
}

void blueThread(void* arg1) 
{ 
  while(DEF_TRUE)
  {
    BlueToggle();
//		OSTimeDly(DELAY_MS/2, OS_OPT_TIME_DLY, &osError);
  } 
}

void greenThread(void* arg1) 
{ 
  while(DEF_TRUE)
  {
    GreenToggle();
//		OSTimeDly(DELAY_MS/4, OS_OPT_TIME_DLY, &osError);
  } 
}

void orangeThread(void* arg1) 
{ 
  while(DEF_TRUE)
  {
    OrangeToggle();
//		OSTimeDly(DELAY_MS/8, OS_OPT_TIME_DLY, &osError);
  } 
}

int main(void)
{
  GPIO_Init();

  OSInit(&osError);
  OSTaskCreate(
               &osRedThreadTcb,    /* TCB */
               "red thread",       /* Descriptive name */
               redThread,          /* Task function */
               (void *) 0,         /* Task function arguments */
               1,                  /* Priority */
               &redThreadStack[0], /* Stack */
               0U,                 /* Stack limit */
               STACK_SIZE,         /* Stack size */
               0,                  /* Msg queue size */
               0,                  /* Time quanta, 0=default */
               0,                  /* TCB extension */
               (OS_OPT_TASK_STK_CHK + OS_OPT_TASK_STK_CLR), /* Options */
               &osError            /* Error */
              );
  OSStart(&osError);			 
}
