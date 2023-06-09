/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file board.c
 * @brief relative configure for xidatong-arm32
 * @version 2.0
 * @author AIIT XUOS Lab
 * @date 2022.03.15
 */

/*************************************************
File name: board.c
Description: support imxrt1176-board init function
Others: take SDK_2.6.1_MIMXRT1052xxxxB for references
History: 
1. Date: 2022-08-19
Author: AIIT XUOS Lab
Modification: 
1. support imxrt1176-board MPU、clock、memory init
2. support imxrt1176-board uart、sdio driver init
*************************************************/

#include <board.h>

#ifdef BSP_USING_GPIO
#include <connect_gpio.h>
#endif

#ifdef BSP_USING_LPUART
#include <connect_uart.h>
#endif

#ifdef BSP_USING_LWIP
extern int ETH_BSP_Config();
#endif

#if __CORTEX_M == 7
void BOARD_ConfigMPU(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t size          = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                        0 :
                        ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
#if defined(__USE_SHMEM)
    extern uint32_t __base_rpmsg_sh_mem;
    extern uint32_t __top_rpmsg_sh_mem;
    uint32_t nonCacheStart = (uint32_t)(&__base_rpmsg_sh_mem);
    uint32_t size          = (uint32_t)(&__top_rpmsg_sh_mem) - nonCacheStart;
#else
    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t size          = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;
#endif
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START;
    extern uint32_t __NCACHE_REGION_END;
    uint32_t nonCacheStart = (uint32_t)(&__NCACHE_REGION_START);
    uint32_t size          = (uint32_t)(&__NCACHE_REGION_END) - nonCacheStart;
#endif
    volatile uint32_t i = 0;

#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }
#endif

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in mpu_armv7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in mpu_armv7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     * back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     * through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     * back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     * noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     * noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     * back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     * back write/read acllocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache
     * policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
     * mpu_armv7.h.
     */

    /*
     * Add default region to deny access to whole address space to workaround speculative prefetch.
     * Refer to Arm errata 1013783-B for more details.
     *
     */
    /* Region 0 setting: Instruction access disabled, No data access permission. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(1, ARM_MPU_AP_NONE, 0, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);

    /* Region 1 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 2 setting: Memory with Device type, not shareable,  non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x20300000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x30000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_16MB);
#endif

#ifdef USE_SDRAM
    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64MB);
#endif

    while ((size >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(nonCacheStart % size));
        assert(size == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 10 setting: Memory with Normal type, not shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(10, nonCacheStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, i - 1);
    }

    /* Region 11 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(11, 0x40000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_16MB);

    /* Region 12 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(12, 0x41000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);

    /* Region 13 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(13, 0x41400000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 14 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(14, 0x41800000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);

    /* Region 15 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(15, 0x42000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_EnableDCache();
#endif
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    SCB_EnableICache();
#endif
}
#elif __CORTEX_M == 4
void BOARD_ConfigMPU(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t nonCacheSize = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                                0 :
                                ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t nonCacheSize  = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t nonCacheSize  = (uint32_t)__NCACHE_REGION_SIZE;
#endif
#if defined(__USE_SHMEM)
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RPMSG_SH_MEM$$Base[];
    /* RPMSG_SH_MEM_unused is a auxiliary region which is used to get the whole size of RPMSG_SH_MEM section */
    extern uint32_t Image$$RPMSG_SH_MEM_unused$$Base[];
    extern uint32_t Image$$RPMSG_SH_MEM_unused$$ZI$$Limit[];
    uint32_t rpmsgShmemStart = (uint32_t)Image$$RPMSG_SH_MEM$$Base;
    uint32_t rpmsgShmemSize = (uint32_t)Image$$RPMSG_SH_MEM_unused$$ZI$$Limit - rpmsgShmemStart;
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_rpmsg_sh_mem;
    extern uint32_t __top_rpmsg_sh_mem;
    uint32_t rpmsgShmemStart = (uint32_t)(&__base_rpmsg_sh_mem);
    uint32_t rpmsgShmemSize  = (uint32_t)(&__top_rpmsg_sh_mem) - rpmsgShmemStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __RPMSG_SH_MEM_START[];
    extern uint32_t __RPMSG_SH_MEM_SIZE[];
    uint32_t rpmsgShmemStart = (uint32_t)__RPMSG_SH_MEM_START;
    uint32_t rpmsgShmemSize  = (uint32_t)__RPMSG_SH_MEM_SIZE;
#endif
#endif
    uint32_t i = 0;

    /* Only config non-cacheable region on system bus */
    assert(nonCacheStart >= 0x20000000);

    /* Disable code bus cache */
    if (LMEM_PCCCR_ENCACHE_MASK == (LMEM_PCCCR_ENCACHE_MASK & LMEM->PCCCR))
    {
        /* Enable the processor code bus to push all modified lines. */
        LMEM->PCCCR |= LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK | LMEM_PCCCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((LMEM->PCCCR & LMEM_PCCCR_GO_MASK) != 0U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        LMEM->PCCCR &= ~(LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK);
        /* Now disable the cache. */
        LMEM->PCCCR &= ~LMEM_PCCCR_ENCACHE_MASK;
    }

    /* Disable system bus cache */
    if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
    {
        /* Enable the processor system bus to push all modified lines. */
        LMEM->PSCCR |= LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK | LMEM_PSCCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((LMEM->PSCCR & LMEM_PSCCR_GO_MASK) != 0U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        LMEM->PSCCR &= ~(LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK);
        /* Now disable the cache. */
        LMEM->PSCCR &= ~LMEM_PSCCR_ENCACHE_MASK;
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    while ((nonCacheSize >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(nonCacheStart % nonCacheSize));
        assert(nonCacheSize == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 0 setting: Memory with device type, not shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(0, nonCacheStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, i - 1);
    }

#if defined(__USE_SHMEM)
    i = 0;

    while ((rpmsgShmemSize >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(rpmsgShmemStart % rpmsgShmemSize));
        assert(rpmsgShmemSize == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 1 setting: Memory with device type, not shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(1, rpmsgShmemStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, i - 1);
    }
#endif

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enables the processor system bus to invalidate all lines in both ways.
    and Initiate the processor system bus cache command. */
    LMEM->PSCCR |= LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK | LMEM_PSCCR_GO_MASK;
    /* Wait until the cache command completes */
    while ((LMEM->PSCCR & LMEM_PSCCR_GO_MASK) != 0U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    LMEM->PSCCR &= ~(LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK);
    /* Now enable the system bus cache. */
    LMEM->PSCCR |= LMEM_PSCCR_ENCACHE_MASK;

    /* Enables the processor code bus to invalidate all lines in both ways.
    and Initiate the processor code bus code cache command. */
    LMEM->PCCCR |= LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_GO_MASK;
    /* Wait until the cache command completes. */
    while ((LMEM->PCCCR & LMEM_PCCCR_GO_MASK) != 0U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    LMEM->PCCCR &= ~(LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK);
    /* Now enable the code bus cache. */
    LMEM->PCCCR |= LMEM_PCCCR_ENCACHE_MASK;
}
#endif

void BOARD_InitModuleClock(void)
{
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);

    clock_root_config_t rootCfg = {.mux = 4, .div = 10};

#ifdef BOARD_NETWORK_USE_100M_ENET_PORT
     /* Generate 50M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet1, &rootCfg);
#endif

#ifdef BOARD_NETWORK_USE_1G_ENET_PORT
    /* Generate 125M root clock. */
    rootCfg.mux = 4;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Enet2, &rootCfg);
#endif

    /* Select syspll2pfd3, 528*18/24 = 396M */
    CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd3, 24);
    rootCfg.mux = 7;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Bus, &rootCfg); /* Generate 198M bus clock. */
}

void IOMUXC_SelectENETClock(void)
{
#ifdef BOARD_NETWORK_USE_100M_ENET_PORT
    IOMUXC_GPR->GPR4 |= 0x3; /* 50M ENET_REF_CLOCK output to PHY and ENET module. */
#endif

#ifdef BOARD_NETWORK_USE_1G_ENET_PORT
    IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK; /* bit1:iomuxc_gpr_enet_clk_dir
                                                                 bit0:GPR_ENET_TX_CLK_SEL(internal or OSC) */
#endif
}
/*!
 * @brief Utility function for comparing arrays
 */
static uint8_t compareArrays(uint8_t a[], uint8_t b[], int len)
{
	for (int i=0; i<len; i++)
	{
		if (a[i] != b[i])
		{
			return 0;
		}
	}
	return 1;
}

/*!
 * @brief configure miiMode, miiSpeed based on the MAC address
 */
void BOARD_ENETFlexibleConfigure(enet_config_t *config, uint8_t *hwAddr)
{
	uint8_t temp_arr[6] = configMAC_ADDR;

	if (compareArrays(hwAddr, temp_arr, 6))
	{
		config->miiMode = kENET_RmiiMode;
		config->miiSpeed = kENET_MiiSpeed100M;
	}
	else
	{
		config->miiMode = kENET_RgmiiMode;
		config->miiSpeed = kENET_MiiSpeed1000M;
	}
}

/* This is the timer interrupt service routine. */
void SysTick_Handler(int irqn, void *arg)
{
    TickAndTaskTimesliceUpdate();
}
DECLARE_HW_IRQ(SYSTICK_IRQN, SysTick_Handler, NONE);

struct InitSequenceDesc _board_init[] = 
{
#ifdef BSP_USING_GPIO
    // { "hw_pin", Imxrt1052HwGpioInit },
#endif

#ifdef BSP_USING_LWIP
    {"ETH_BSP", ETH_BSP_Config},
#endif

	{ " NONE ",NONE },
};

/**
 * This function will initial imxrt1050 board.
 */
void InitBoardHardware()
{
    int i = 0;
	int ret = 0;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitModuleClock();
    IOMUXC_SelectENETClock();

#ifdef BOARD_NETWORK_USE_100M_ENET_PORT
    BOARD_InitEnetPins();
#endif
#ifdef BOARD_NETWORK_USE_1G_ENET_PORT
    BOARD_InitEnet1GPins();
#endif

    // NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    SysTick_Config(SystemCoreClock / TICK_PER_SECOND);

    InitBoardMemory((void *)HEAP_BEGIN, (void *)HEAP_END);

#ifdef BSP_USING_LPUART
    Imxrt1176HwUartInit();
#endif

    InstallConsole(KERNEL_CONSOLE_BUS_NAME, KERNEL_CONSOLE_DRV_NAME, KERNEL_CONSOLE_DEVICE_NAME);
    KPrintf("\nconsole init completed.\n");
    KPrintf("board initialization......\n");

    for(i = 0; _board_init[i].fn != NONE; i++) {
		ret = _board_init[i].fn();
		KPrintf("initialize %s %s\n",_board_init[i].fn_name, ret == 0 ? "success" : "failed");
	}
    KPrintf("board init done.\n");
	KPrintf("start kernel...\n");
}

