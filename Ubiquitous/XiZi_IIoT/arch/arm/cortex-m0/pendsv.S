/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-11     Bernard      first version
 * 2012-01-01     aozima       support context switch load/store FPU register.
 * 2013-06-18     aozima       add restore MSP feature.
 * 2013-06-23     aozima       support lazy stack optimized.
 * 2018-07-24     aozima       enhancement hard fault exception handler.
 */

/*************************************************
File name: pendsv.S
Description: PendSV interrupt handler
Others: take RT-Thread v4.0.2/libcpu/arm/cortex-m4/context_gcc.S for references
        https://github.com/RT-Thread/rt-thread/tree/v4.0.2
History: 
1. Date: 2021-04-25
Author: AIIT XUOS Lab
*************************************************/

#include <xsconfig.h>

.cpu cortex-m0
.syntax unified
.thumb
.text

.equ    SCB_VTOR,           0xE000ED08
.equ    NVIC_INT_CTRL,      0xE000ED04
.equ    NVIC_SYSPRI2,       0xE000ED20
.equ    NVIC_PENDSV_PRI,    0x00FF0000
.equ    NVIC_PENDSVSET,     0x10000000

.globl PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
    MRS r3, PRIMASK
    CPSID   I

    LDR r0, =KtaskSwitchInterruptFlag
    LDR r1, [r0]
    /*CBZ r1, switch_to_task*/
    CMP r1, #0
    BEQ pendsv_exit

    MOVS r1, #0x00
    STR r1, [r0]

    LDR r0, =InterruptFromKtask
    LDR r1, [r0]
    /*CBZ r1, switch_to_task*/
    CMP r1, #0
    BEQ switch_to_task

    MRS r1, psp

    /*STMFD   r1!, {r3 - r11}*/
    SUBS    r1, #0x24
    STMIA   r1!, {r3 - r7}
    MOV     r3, r8
    MOV     r4, r9
    MOV     r5, r10
    MOV     r6, r11
    STMIA   r1!, {r3 - r6}
    SUBS     r1, #0x24

#if defined (__VFP_FP__) && !defined(__SOFTFP__)
    MOV     r4, #0x00

    TST     lr, #0x10
    MOVEQ   r4, #0x01

    /*STMFD   r1!, {r4}*/
    SUBS    r1, #0x4
    STMIA   r1!, {r4}
    SUBS    r1, #0x4
#endif

    LDR r0, [r0]
    STR r1, [r0]

switch_to_task:

    PUSH    {lr}
    BL      UpdateRunningTask
    POP     {r0}
    MOV     lr, r0

#ifdef TASK_ISOLATION
    PUSH    {lr}
    BL      GetTaskPrivilege
    /*POP     {lr}*/
    POP     {r0}
    MOV     lr, r0
#endif

    LDR r1, =InterruptToKtask
    LDR r1, [r1]
    LDR r1, [r1]

#if defined (__VFP_FP__) && !defined(__SOFTFP__)
    LDMFD   r1!, {r2}
#endif

    /*LDMFD   r1!, {r3 - r11}*/
    ADDS    r1, #0x14
    LDMFD   r1!, {r3 - r6}
    MOV     r8, r3
    MOV     r9, r4
    MOV     r10, r5
    MOV     r11, r6
    SUBS    r1, #0x24
    LDMFD   r1!, {r3 - r7}
    ADDS    r1, #0x10

    MSR psp, r1
#if defined (__VFP_FP__) && !defined(__SOFTFP__)
    /*ORR     lr, lr, #0x10*/
    MOV     r2, lr
    MOVS    r3, #0x10
    ORRS    r2, r3
    MOV     lr, r2

    CMP     r2,  #0
    BICNE   lr, lr, #0x10
#endif
    MRS		r2, control
#ifdef TASK_ISOLATION
    CMP     r0, #1
    BEQ     unprivilege

privilege:
    BIC		r2, r2, #0x01
    B       exit
unprivilege:
    /*ORR		r2, r2, #0x01*/
    MOVS    r1, #0x01
    ORRS    r2, r1
#else
    /*BIC		r2, r2, #0x01*/
    MOVS    r0, #0x01
    BICS	r2, r0
#endif
exit:
    MSR	    control, r2

pendsv_exit:
    /*ORR lr, lr, #0x04*/
    MOV     r0, lr
    MOVS    r1, #0x04
    ORRS     r0, r1
    MOV     lr, r0

    MSR     PRIMASK, r3
    BX      lr
