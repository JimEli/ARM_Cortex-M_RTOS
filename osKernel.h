#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

#include "stm32f4xx.h" // Device header
#include <stdint.h>

#ifdef __cplusplus
extern "C"
#endif

// Function prototypes.
void osSchedulerStart(void); 
void osKernelInit(void);
void osKernelAddThread(void(*)(void));
int8_t osKernelStart(uint32_t);
void osSuspendThread(void);
void osSemaphoreInit(int32_t*, int32_t);
void osSignalSet(int32_t*);
void osSignalWait(int32_t*);
uint32_t getTick(void);
void Delay(uint32_t);

// 10 ms. time slice alloted per task/thread.
#define TIME_SLICE	10

#define NUM_THREADS 4        // Number of demonstration threads.
#define STACKSIZE   100      // Size of each thread stack.
#define BUS_FREQ    16000000 // Used to calculate prescale value (16MHz).

// SYSPRI3 register configures priority level (0-7) of SysTick exception handler.
// A priority level >= 1 is required to enable SysTick interrupts.
#define SYSPRI3 (*((volatile uint32_t *)0xE000ED20))
// Interrupt Control and State Register (ICSR), Bit 26 is set-pending for SysTick exception.
#define INTCTRL (*((volatile uint32_t *)0xE000ED04))

// SysTick counter value (ms).
uint32_t MILLIS_PRESCALER;          

// Thread Control Block.
struct tcb
{
	int32_t* stackPt;       
	struct tcb* nextPt;  
};

typedef struct tcb tcbType;
tcbType tcbs[NUM_THREADS];
tcbType* currentPt;
int32_t TCB_STACK[NUM_THREADS][STACKSIZE];

// Number of threads.
uint32_t threadCount;

void osKernelInit(void)
{
	__disable_irq();
	// Convert systick counter value to ms.
	MILLIS_PRESCALER = (BUS_FREQ / 1000);
	// No threads created yet.
	threadCount = 0;
}

void osKernelStackInit(int threadID, void(*thread)(void))
{
	// Set xPSR
	tcbs[threadID].stackPt = &TCB_STACK[threadID][STACKSIZE - 16]; 
	// Set thumb mode.
	TCB_STACK[threadID][STACKSIZE - 1] = 0x01000000; 
	// Set task function address.
	TCB_STACK[threadID][STACKSIZE - 2] = (int32_t)(thread); 
}

void osKernelAddThread(void(*thread)(void))
{
	// Reached max thread count?
	if (threadCount >= NUM_THREADS)
		return;
	// Atomic section. 
	__disable_irq(); 
	// Link threads round robin style.
	if (threadCount)
		// Link previous thread to new one.
		tcbs[(threadCount - 1)].nextPt = &tcbs[threadCount];
	else
		// Set current thread pointer to 1st thread.
		currentPt = &tcbs[0];
	// Make list circular.
	tcbs[threadCount].nextPt = &tcbs[0]; 
	// Init thread stack.
	osKernelStackInit(threadCount, thread);
	// Increment thread count.
	threadCount++;
	__enable_irq();
}

int8_t osKernelStart(uint32_t quanta)
{
	if (!threadCount)
		// Nothing to launch.
		return - 1;

	// Disable systick.
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	// Systick countdown value.
	SysTick->LOAD = (quanta * MILLIS_PRESCALER) - 1;
	// Set lowest priority (7).
	SYSPRI3 = (SYSPRI3 & 0x00FFFFFF) | 0xE0000000;
	// Enable systick.
	SysTick->CTRL = 0x00000007;
	
	// Launch the system.
	osSchedulerStart();
	
	return 1;
}

// Yield thread.
void osSuspendThread(void)
{
	// Reset counter for next thread.
	SysTick->VAL = 0;
	// Trigger systick interrupt.
	INTCTRL = 0x0400000;
}

// Initialize cooperative-spinlock semaphore.
void osSemaphoreInit(int32_t* semaphore, int32_t n) { *semaphore = n; }

void osSignalSet(int32_t* semaphore)
{
	__disable_irq();
	*semaphore += 1;
	__enable_irq();
}

void osSignalWait(int32_t* semaphore)
{
	__disable_irq();
	while (*semaphore <= 0)
	{
		__enable_irq();
		// Cooperative-semaphore adds yield function.
		osSuspendThread();
		__disable_irq();
	}
	*semaphore -= 1;
	__enable_irq();
}

// Count of sysTick ticks.
volatile uint32_t ticks; 
volatile uint32_t _ticks;

uint32_t getTick()
{
	__disable_irq();
	_ticks = ticks;
	__enable_irq();
	return _ticks;
}

// Delay number of Systicks
void Delay(uint32_t ms) 
{
	uint32_t start;

	start = getTick();
	while ((getTick() - start) < ms) {}
}

#endif
