/**
  *************** (C) COPYRIGHT 2017 STMicroelectronics ************************
  * @file      startup_stm32f103xb.s
  * @author    MCD Application Team
  * @brief     STM32F103xB Devices vector table for Atollic toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Configure the clock system   
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M3 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/**
* @file boot.S
* @brief derived from ST standard peripheral library
* @version 1.1 
* @author AIIT XUOS Lab
* @date 2021-11-30
*/

/*************************************************
File name: boot.S
Description: Reset and init function
Others: 
History: 
1. Date: 2021-11-30
Author: AIIT XUOS Lab
Modification: 
1. take startup_stm32f103xb.s for XiZi kernel
*************************************************/

  .syntax unified
  .cpu cortex-m3
  .fpu softvfp
  .thumb

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

.equ  BootRAM, 0xF108F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:

/* Copy the data segment initializers from flash to SRAM */
  movs r1, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r3, =_sidata
  ldr r3, [r3, r1]
  str r3, [r0, r1]
  adds r1, r1, #4

LoopCopyDataInit:
  ldr r0, =_sdata
  ldr r3, =_edata
  adds r2, r0, r1
  cmp r2, r3
  bcc CopyDataInit
  ldr r2, =_sbss
  b LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
  movs r3, #0
  str r3, [r2], #4

LoopFillZerobss:
  ldr r3, = _ebss
  cmp r2, r3
  bcc FillZerobss

/* Call the clock system intitialization function.*/
    bl  SystemInit

/* Call the application's entry point.*/
  bl entry
  bx lr
.size Reset_Handler, .-Reset_Handler
