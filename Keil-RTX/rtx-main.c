// STM4F07 Discovery Keil-RTX Demonstration.
#include "cmsis_os.h"  // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx.h" // Device header

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

void BlueOn(void) { GPIOD->ODR |= BLUE; }
void BlueOff(void) { GPIOD->ODR &= ~BLUE; }
void OrangeOn(void) { GPIOD->ODR |= ORANGE; }
void OrangeOff(void) { GPIOD->ODR &= ~ORANGE; }
void RedToggle(void) { GPIOD->ODR ^= RED; }
void BlueToggle(void) { GPIOD->ODR ^= BLUE; }
void GreeToggle(void) { GPIOD->ODR ^= GREEN; }
void OrangeToggle(void) { GPIOD->ODR ^= ORANGE; }

void GPIO_Init()
{
  // Enable clock access to PORT of PIN.
  RCC->AHB1ENR |= PORTD_CLOCK;
  // Set the PIN's mode.
  GPIOD->MODER |= (RED_BIT | GREEN_BIT | BLUE_BIT | ORANGE_BIT);	
}

//int32_t count1 = 0, count2 = 0, count3 = 0;

void orangeThread(void const* arg1)
{
  while(1)
  {
    // Toggle LED.
    GPIOD->ODR ^= ORANGE;
    osDelay(2000);
  }
}

void redThread(void const* arg1)
{
  while(1)  
  {
    //count3++;
    // Toggle LED.
    GPIOD->ODR ^= RED;
    osDelay(1000);
  }
}

void blueThread(void const* arg1)
{
  while(1)
  {
    //count1++;
    // Toggle LED.
    BlueToggle();
    osDelay(500);
  }
}

void greenThread(void const* arg1)
{
  while(1)
  {
    //count2++;
    // Toggle LED.
    GPIOD->ODR |= GREEN;
    osDelay(250);
    GPIOD->ODR &= ~GREEN;
    osDelay(250);
  }
}

// Define single instances of 4 threads, normal priority, defualt stack size.
osThreadDef(orangeThread, osPriorityNormal, 1, 0);
osThreadDef(redThread, osPriorityNormal, 1, 0);
osThreadDef(blueThread, osPriorityNormal, 1, 0);
osThreadDef(greenThread, osPriorityNormal, 1, 0);

int main(void) 
{
  GPIO_Init();

  osThreadCreate(osThread(orangeThread), NULL);
  osThreadCreate(osThread(redThread), NULL);
  osThreadCreate(osThread(blueThread), NULL);
  osThreadCreate(osThread(greenThread), NULL);
  
  while(1);
}	
