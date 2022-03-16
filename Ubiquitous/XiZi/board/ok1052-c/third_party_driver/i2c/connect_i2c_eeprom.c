/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
* @file connect_i2c_eeprom.h
* @brief ok1052-c eeprom relative codes
* @version 1.0
* @author AIIT XUOS Lab
* @date 2022-03-01
*/

#include "board.h"
#include "connect_i2c.h"
#include "fsl_lpi2c.h"
#include "pin_mux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define I2C_EEPROM_BASE  LPI2C1
#define I2C_EEPROM_ADDR (0xA0 >> 1)

/*******************************************************************************
 * Code
 ******************************************************************************/

void I2cEEpromTestWrite(void)
{
    uint8_t dat[8] = {0};

    if(I2cHardwareRead(I2C_EEPROM_BASE, I2C_EEPROM_ADDR, 0, dat, 8) == kStatus_Success)
    {
        i2c_print("Read from EEPROM %d %d %d %d %d %d %d %d\r\n",
            dat[0], dat[1], dat[2], dat[3], dat[4], dat[5], dat[6], dat[7]);
    }

    for(uint8_t i = 0; i < 8; i++)
    {
        dat[i] ++;
    }

    if(I2cHardwareWrite(I2C_EEPROM_BASE, I2C_EEPROM_ADDR, 0, dat, 8) == kStatus_Success)
    {
        i2c_print("Write  to EEPROM %d %d %d %d %d %d %d %d\r\n",
            dat[0], dat[1], dat[2], dat[3], dat[4], dat[5], dat[6], dat[7]);
    }

    memset(dat, 0, 8);
    if(I2cHardwareRead(I2C_EEPROM_BASE, I2C_EEPROM_ADDR, 0, dat, 8) == kStatus_Success)
    {
        i2c_print("Read from EEPROM %d %d %d %d %d %d %d %d\r\n",
            dat[0], dat[1], dat[2], dat[3], dat[4], dat[5], dat[6], dat[7]);
    }
}

int I2cEEpromTest(void)
{
    Stm32HwI2cInit();
    BOARD_InitI2C1Pins();
    I2cHardwareInit();
    I2cEEpromTestWrite();
    return 0;
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)| SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)| SHELL_CMD_PARAM_NUM(0),
                   eeprom, I2cEEpromTest, test i2c eeprom);
