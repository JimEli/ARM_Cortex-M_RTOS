//
// osScheduler (SysTick_Handler) and osSchedulerStart Functions.
//
.syntax unified
.text
.align 2
.thumb
.extern currentPt
.extern ticks

//
// Preemptive periodic interrupt which performs thread context switch.
// Triggered at fixed rate period of QUANTA set in osKernelStart function.
//
.global SysTick_Handler
.type   SysTick_Handler STT_FUNC
SysTick_Handler:           // Interrupt auto saves 8 registers: R0-R3, R12, LR, PC xPSR.
    CPSID  I               // Disable interrupts.
    PUSH   {R4-R11}        // Save remaining 8 registers: R4-R11 to currebt TCB stack.
	                       // Tick counter.
//	LDR    R0, =ticks      // R0 = ticks addr.
//	LDR    R1, [R0]        // R1 = ticks.
//	ADDS   R1, R1, #1      // Increment R1.
//	STR    R1, [R0]        // Save R1 to ticks.
	                       // Perform contex switch.
    LDR    R0, =currentPt  // R0 = currentPt to old thread TCB.
    LDR    R1, [R0]        // R1 = currentPt to old thread TCB.
    STR    SP, [R1]        // Save current stack pointer into old TCB.
    LDR    R1, [R1, #4]    // R1 = currentPt->next.
    STR    R1, [R0]        // CurrentPt = currentPt->next (new TCB).
    LDR    SP, [R1]        // SP = currentPt->stackPt of new TCB.   

    POP    {R4 - R11}      // Restore registers R4-R11.     
    CPSIE  I               // Enable interrupts.
    BX     LR              // Return/restore auto saved registers.

//
// Launch os scheduler.
//
.global osSchedulerStart
.type   osSchedulerStart STT_FUNC
osSchedulerStart:
    LDR    R0, =currentPt  // R0 = currentPt (TCB of initial thread).       
    LDR    R2, [R0]        // R2 = currentPt = TCB of initial thread.
    LDR    SP, [R2]        // SP = currentPt->stackPt of TCB of initial thread.
    POP    {R4 - R11}      // Set inital register values (R4-R11).
    POP    {R0 - R3}       // R0-R3.
    POP    {R12}           // R12.
    ADD    SP, SP, #4      // Discard LR from inital stack.
    POP    {LR}            // Set inital LR (start location).
    ADD    SP, SP, #4      // Discard PSR.
    CPSIE  I               // Enable interrupts.
    BX     LR              // Start first thread...

    .align
    .end
