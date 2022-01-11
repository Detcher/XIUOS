/*
* Copyright (c) 2020 AIIT XUOS Lab
* XiUOS is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*        http://license.coscl.org.cn/MulanPSL2
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
* See the Mulan PSL v2 for more details.
*/
 
/**
* @file connect_dac.h
* @brief define stm32f407-st-discovery adc function and struct
* @version 2.0
* @author AIIT XUOS Lab
* @date 2022-1-10
*/

#ifndef CONNECT_DAC_H
#define CONNECT_DAC_H

#include <device.h>
#include <hardware_dac.h>
#include <hardware_rcc.h>
#include <hardware_gpio.h>

struct Stm32HwDac 
{
    DAC_TypeDef *DACx;
    uint16 digital_data;
};

int Stm32HwDacInit(void);

#endif
