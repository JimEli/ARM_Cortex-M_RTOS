/*
  Round Robin RTOS Scheduler Demo for Cortex-M4.
  STM32F4 Discovery Board version.
  
  James M. Eli
  

  Each thread has a thread control block (TCB) encapsulating the state 
  of the thread. A thread’s TCB maintains a link to its stack and a 
  link to the TCB of the next thread. TCBs are maintained in a circular 
  linked list. CurrentPt points to the TCB of the thread that is currently 
  running. The next field is a pointer chaining all 4 TCBs into a circular
  linked list. Each TCB has a stackPt field. If the thread is running it 
  is using the real SP for its stack pointer. However, the other threads 
  have their stack pointers saved in this field. The scheduler traverses 
  the linked list of TCBs to find the next thread to run. Each thread 
  runs for a fixed amount of time, and a periodic interrupt suspends the 
  running thread and switches CurrentPt to the next thread in the circular 
  list. The scheduler then launches the next thread. While a thread is 
  running, it uses the actual Cortex M hardware registers.
*/
#include <stm32f4xx_hal.h>

#include "osKernel.h"

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

void initGPIO()
{
  // Enable clock access to PORT of PIN.
  RCC->AHB1ENR |= PORTD_CLOCK;
  // Set the PIN's mode.
  GPIOD->MODER |= (RED_BIT | GREEN_BIT | BLUE_BIT | ORANGE_BIT);	
}

static TIM_HandleTypeDef sTimerInstance = { .Instance = TIM2 };

void initTimer()
{
	__TIM2_CLK_ENABLE();
	sTimerInstance.Init.Prescaler = 16000;
	sTimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
	sTimerInstance.Init.Period = 1;
	sTimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	sTimerInstance.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&sTimerInstance);
	HAL_TIM_Base_Start_IT(&sTimerInstance);
	// Set interrupt priority. Preemption/SubPriority for IRQn=0, 
	// where lower indicates higher priority (0-15). 
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

extern "C" void TIM2_IRQHandler() 
{ 
	HAL_TIM_IRQHandler(&sTimerInstance); 
	ticks++;
}

// Thread counters.
uint32_t count0 = 0, count1 = 0, count2 = 0, count3 = 0;

// Demo threads.
void orangeThread(void) { while (1) { /*count0++;*/ GPIOD->ODR ^= ORANGE; Delay(1000); } }
void redThread(void) { while(1) { /*count1++;*/ GPIOD->ODR ^= RED; Delay(500); } }
void blueThread(void) { while(1) { /*count2++;*/ GPIOD->ODR ^= BLUE; Delay(250); } }
void greenThread(void) { while(1) { /*count3++;*/ GPIOD->ODR ^= GREEN; Delay(125); } }

int main(void)
{
  //asm("bkpt 255");

  HAL_Init();
  initTimer();

  initGPIO();
	
  osKernelInit();
  osKernelAddThread(&orangeThread);
  osKernelAddThread(&redThread);
  osKernelAddThread(&blueThread);
  osKernelAddThread(&greenThread);
  osKernelStart(TIME_SLICE);
	
  while(1);
}

/*
// Thread semaphore example.
int32_t semaphore1, semaphore2;

// Demonstration tasks.
void Task1(void) 
{ 
	while (1) 
	{
		// Release task #1 semaphore.
		osSignalSet(&semaphore1);
		// Wait on task #2 semaphore.
		osSignalWait(&semaphore2);
		count1++;
	}
}
void Task2(void)
{ 
	while (1) 
	{
		// Release task #2 semaphore.
		osSignalSet(&semaphore2);
		// Wait on task #1 semaphore.
		osSignalWait(&semaphore1);
		count2++;
	}
}

// Initalize semaphores in main().
osSemaphoreInit(&semaphore1, 0);
osSemaphoreInit(&semaphore2, 0);
*/
