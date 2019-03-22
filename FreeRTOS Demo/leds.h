// STM32F407 Discovery LED functions.

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

void BlueOn(void);
void BlueOff(void);
void OrangeOn(void);
void OrangeOff(void);
void RedToggle(void);
void BlueToggle(void);
void GreenToggle(void);
void OrangeToggle(void);

void GPIO_Init(void);
