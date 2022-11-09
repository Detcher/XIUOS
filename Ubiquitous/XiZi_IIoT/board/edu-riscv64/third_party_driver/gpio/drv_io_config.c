/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-03-19     ZYH          first version
 */

/**
* @file drv_io_config.c
* @brief support edu-riscv64-board io configure
* @version 2.0 
* @author AIIT XUOS Lab
* @date 2022-10-17
*/

/*************************************************
File name: drv_io_config.c
Description: support kd233-board io configure
Others: take RT-Thread v4.0.2/bsp/k210/driver/drv_io_config.c for references
                https://github.com/RT-Thread/rt-thread/tree/v4.0.2
History: 
1. Date: 2022-10-17
Author: AIIT XUOS Lab
Modification: support kd233-board io configure
*************************************************/

#include <xizi.h>
#include <fpioa.h>
#include "drv_io_config.h"
#include <sysctl.h>           

#define HS_GPIO(n) (FUNC_GPIOHS0 + n)

#define IOCONFIG(pin,func)  {pin, func, #func}

static struct io_config
{
    int io_num;
    fpioa_function_t func;
    const char * FuncName;
} io_config[] = 
{
#ifdef BSP_USING_LCD
    IOCONFIG(BSP_LCD_CS_PIN, FUNC_SPI0_SS0),                
    IOCONFIG(BSP_LCD_WR_PIN, FUNC_SPI0_SCLK),                
    IOCONFIG(BSP_LCD_DC_PIN, HS_GPIO(FPIOA_LCD_DC)),
    IOCONFIG(BSP_LCD_RST_PIN, HS_GPIO(FPIOA_LCD_RST)),     
#endif

#ifdef BSP_USING_SPI1
    IOCONFIG(BSP_SPI1_CLK_PIN, FUNC_SPI1_SCLK),
    IOCONFIG(BSP_SPI1_D0_PIN, FUNC_SPI1_D0),
    IOCONFIG(BSP_SPI1_D1_PIN, FUNC_SPI1_D1),
#ifdef BSP_USING_SPI1_AS_QSPI
    IOCONFIG(BSP_SPI1_D2_PIN, FUNC_SPI1_D2),
    IOCONFIG(BSP_SPI1_D3_PIN, FUNC_SPI1_D3),
#endif
#ifdef BSP_SPI1_USING_SS0
    IOCONFIG(BSP_SPI1_SS0_PIN, HS_GPIO(SPI1_CS0_PIN)),
#endif
#endif

#ifdef BSP_USING_UART1
    IOCONFIG(BSP_UART1_TXD_PIN, FUNC_UART1_TX),
    IOCONFIG(BSP_UART1_RXD_PIN, FUNC_UART1_RX),
#endif
#ifdef BSP_USING_UART2
    IOCONFIG(BSP_UART2_TXD_PIN, FUNC_UART2_TX),
    IOCONFIG(BSP_UART2_RXD_PIN, FUNC_UART2_RX),
#endif
#ifdef BSP_USING_UART3
    IOCONFIG(BSP_UART3_TXD_PIN, FUNC_UART3_RX),
    IOCONFIG(BSP_UART3_RXD_PIN, FUNC_UART3_TX),
#endif
#ifdef BSP_USING_I2C1
    IOCONFIG(BSP_I2C_SDA, FUNC_GPIO3),
    IOCONFIG(BSP_I2C_SCL, FUNC_GPIO4),
#endif
#ifdef BSP_USING_TOUCH
    IOCONFIG(BSP_TOUCH_TP_INT, HS_GPIO(FPIOA_TOUCH_TP_INT)),
#endif

#ifdef BSP_USING_CH438
    IOCONFIG(BSP_CH438_ALE_PIN,  HS_GPIO(FPIOA_CH438_ALE)),      
    IOCONFIG(BSP_CH438_NWR_PIN,  HS_GPIO(FPIOA_CH438_NWR)),      
    IOCONFIG(BSP_CH438_NRD_PIN,  HS_GPIO(FPIOA_CH438_NRD)),      
    IOCONFIG(BSP_CH438_INT_PIN,  HS_GPIO(FPIOA_CH438_INT)),      
    IOCONFIG(BSP_CH438_D0_PIN,  HS_GPIO(FPIOA_CH438_D0)),      
    IOCONFIG(BSP_CH438_D1_PIN,  HS_GPIO(FPIOA_CH438_D1)),      
    IOCONFIG(BSP_CH438_D2_PIN,  HS_GPIO(FPIOA_CH438_D2)),      
    IOCONFIG(BSP_CH438_D3_PIN,  HS_GPIO(FPIOA_CH438_D3)),      
    IOCONFIG(BSP_CH438_D4_PIN,  HS_GPIO(FPIOA_CH438_D4)),      
    IOCONFIG(BSP_CH438_D5_PIN,  HS_GPIO(FPIOA_CH438_D5)),      
    IOCONFIG(BSP_CH438_D6_PIN,  HS_GPIO(FPIOA_CH438_D6)),      
    IOCONFIG(BSP_CH438_D7_PIN,  HS_GPIO(FPIOA_CH438_D7)),
#endif

#ifdef BSP_USING_SOFT_SPI
    IOCONFIG(BSP_SOFT_SPI_SCK_PIN,  HS_GPIO(FPIOA_SOFT_SPI_SCK)),      
    IOCONFIG(BSP_SOFT_SPI_MIOS_PIN,  HS_GPIO(FPIOA_SOFT_SPI_MIOS)),      
    IOCONFIG(BSP_SOFT_SPI_MSOI_PIN,  HS_GPIO(FPIOA_SOFT_SPI_MSOI)),      
    IOCONFIG(BSP_SOFT_SPI_NCS_PIN,  HS_GPIO(FPIOA_SOFT_SPI_NCS)),
#endif

#ifdef BSP_USING_LED
    IOCONFIG(BSP_LED_PIN,FUNC_GPIO5);
#endif

#ifdef BSP_USING_KEY
    IOCONFIG(BSP_KEY_PIN,FUNC_GPIO6);
#endif

};

static int PrintIoConfig()
{
    int i;
    KPrintf("IO Configuration Table\n");
    KPrintf("┌───────┬────────────────────────┐\n");
    KPrintf("│Pin    │Function                │\n");
    KPrintf("├───────┼────────────────────────┤\n");
    for(i = 0; i < sizeof io_config / sizeof io_config[0]; i++)
    {
        KPrintf("│%-2d     │%-24.24s│\n", io_config[i].io_num, io_config[i].FuncName);
    }
    KPrintf("└───────┴────────────────────────┘\n");
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_PARAM_NUM(0),
                                                io,PrintIoConfig,print io config);

int IoConfigInit(void)
{
    int count = sizeof(io_config) / sizeof(io_config[0]);
    int i;
    int ret = 0;

    sysctl_set_power_mode(SYSCTL_POWER_BANK0, SYSCTL_POWER_V18);
    sysctl_set_power_mode(SYSCTL_POWER_BANK1, SYSCTL_POWER_V18);
    sysctl_set_power_mode(SYSCTL_POWER_BANK2, SYSCTL_POWER_V18);
#ifdef BSP_USING_UART2
    // for IO-27/28
    sysctl_set_power_mode(SYSCTL_POWER_BANK4, SYSCTL_POWER_V33);
#endif
#if  defined(BSP_USING_UART1) || defined(BSP_USING_UART3)
    // for IO-20~23
    sysctl_set_power_mode(SYSCTL_POWER_BANK3, SYSCTL_POWER_V33);
#endif

    for(i = 0; i < count; i++)
    {
        ret = FpioaSetFunction(io_config[i].io_num, io_config[i].func);
        if(ret != 0)
            return ret;
    }

    return ret;
}

