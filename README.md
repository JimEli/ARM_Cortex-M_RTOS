# ARM_Cortex-M_Basic_RTOS
## Basic Round Robin RTOS Scheduler Demonstration for Cortex-M4

Utilizes an STM32F407VG Discovery Board.

RTOS functionaltiy includes:
* A Round-Robin OS Scheduler
* Contex Switching via Systick ISR
* Thread Creation
* Thread Suspension
* Spin-lock Semaphore
* Tick/Delay Timer IRQ 

Background

Each thread has a thread control block (TCB) encapsulating the state of the thread. A thread’s TCB maintains a link to its stack and a link to the TCB of the next thread. TCBs are maintained in a circular linked list. CurrentPt points to the TCB of the thread that is currently running. The next field is a pointer chaining all 4 TCBs into a circularlinked list. Each TCB has a stackPt field. If the thread is running it is using the real SP for its stack pointer. However, the other threads have their stack pointers saved in this field. The scheduler traverses the linked list of TCBs to find the next thread to run. Each thread runs for a fixed amount of time, and a periodic interrupt suspends the running thread and switches CurrentPt to the next thread in the circular list. The scheduler then launches the next thread. While a thread is running, it uses the actual Cortex M hardware registers.

Program was developed with Keil and VisualGDB/MS Visual Studio IDEs.
