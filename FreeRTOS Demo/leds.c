// STM32F407 Discovery LED functions.

#include "leds.h"

void BlueOn(void) { GPIOD->ODR |= BLUE; }
void BlueOff(void) { GPIOD->ODR &= ~BLUE; }
void OrangeOn(void) { GPIOD->ODR |= ORANGE; }
void OrangeOff(void) { GPIOD->ODR &= ~ORANGE; }
void RedToggle(void) { GPIOD->ODR ^= RED; }
void BlueToggle(void) { GPIOD->ODR ^= BLUE; }
void GreenToggle(void) { GPIOD->ODR ^= GREEN; }
void OrangeToggle(void) { GPIOD->ODR ^= ORANGE; }

void GPIO_Init(void)
{
	// Enable clock access to PORT of PIN.
	RCC->AHB1ENR |= PORTD_CLOCK;
  // Set the PIN's mode.
  GPIOD->MODER |= (RED_BIT | GREEN_BIT | BLUE_BIT | ORANGE_BIT);	
}
