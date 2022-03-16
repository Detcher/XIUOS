/*
 * Copyright (c) 2020 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2012-04-25     weety         first version
 */

/**
* @file connect_i2c.c
* @brief support ok1052-c i2c function and register to bus framework
* @version 1.0
* @author AIIT XUOS Lab
* @date 2022-03-01
*/

/*************************************************
File name: connect_i2c.c
Description: support ok1052-c i2c configure and i2c bus register function
Others: take RT-Thread v4.0.2/components/drivers/i2c/i2c-bit-ops.c for references
                https://github.com/RT-Thread/rt-thread/tree/v4.0.2
History:
1. Date: 2022-03-01
Author: AIIT XUOS Lab
Modification:
1. support ok1052-c i2c bit configure, write and read
2. support ok1052-c i2c bus device and driver register
*************************************************/

#include <board.h>
#include "bus_serial.h"
#include "connect_i2c.h"
#include "fsl_lpi2c.h"

#ifndef BSP_USING_I2C1
#define BSP_USING_I2C1
#endif

static uint32 I2cWriteData(struct I2cHardwareDevice *i2c_dev, struct I2cDataStandard *msg)
{
    status_t ret;
    Stm32I2cType *param = (Stm32I2cType *)i2c_dev->haldev.private_data;
    ret = I2cHardwareWrite(param->base, param->slave_addr, param->sub_addr, msg->buf, msg->len);
    if(kStatus_Success == ret)
        return 1;
    return 0;
}

static uint32 I2cReadData(struct I2cHardwareDevice *i2c_dev, struct I2cDataStandard *msg)
{
    status_t ret;
    Stm32I2cType *param = (Stm32I2cType *)i2c_dev->haldev.private_data;
    ret = I2cHardwareRead(param->base, i2c_dev->i2c_dev_addr, param->sub_addr, msg->buf, msg->len);
    if(kStatus_Success == ret)
        return 1;
    return 0;
}

static uint32 I2cInit(struct I2cDriver *i2c_drv, struct BusConfigureInfo *configure_info)
{
    NULL_PARAM_CHECK(i2c_drv);

    struct I2cHardwareDevice *i2c_dev = (struct I2cHardwareDevice *)i2c_drv->driver.owner_bus->owner_haldev;

    if (configure_info->private_data) {
        i2c_dev->i2c_dev_addr = *((uint16 *)configure_info->private_data);
        return EOK;
    }

    i2c_print("I2cInit need set i2c dev addr\n");
    return ERROR;
}

static uint32 I2cDrvConfigure(void *drv, struct BusConfigureInfo *configure_info)
{
    NULL_PARAM_CHECK(drv);
    NULL_PARAM_CHECK(configure_info);

    x_err_t ret = EOK;
    struct I2cDriver *i2c_drv = (struct I2cDriver *)drv;

    switch (configure_info->configure_cmd)
    {
        case OPE_INT:
            ret = I2cInit(i2c_drv, configure_info);
            break;
        default:
            break;
    }

    return ret;
}

/*manage the i2c device operations*/
static const struct I2cDevDone i2c_dev_done =
{
    .dev_open = NONE,
    .dev_close = NONE,
    .dev_write = I2cWriteData,
    .dev_read = I2cReadData,
};

/*Init i2c bus*/
static int BoardI2cBusInit(struct I2cBus *i2c_bus, struct I2cDriver *i2c_driver)
{
    x_err_t ret = EOK;

    /*Init the i2c bus */
    i2c_bus->private_data = (void *)NULL;
    ret = I2cBusInit(i2c_bus, I2C_BUS_NAME_1);
    if (EOK != ret) {
        i2c_print("BoardI2cBusInit I2cBusInit error %d\n", ret);
        return ERROR;
    }

    /*Init the i2c driver*/
    i2c_driver->private_data = (void *)NULL;
    ret = I2cDriverInit(i2c_driver, I2C_DRV_NAME_1);
    if (EOK != ret) {
        i2c_print("BoardI2cBusInit I2cDriverInit error %d\n", ret);
        return ERROR;
    }

    /*Attach the i2c driver to the i2c bus*/
    ret = I2cDriverAttachToBus(I2C_DRV_NAME_1, I2C_BUS_NAME_1);
    if (EOK != ret) {
        i2c_print("BoardI2cBusInit I2cDriverAttachToBus error %d\n", ret);
        return ERROR;
    }

    return ret;
}

/*Attach the i2c device to the i2c bus*/
static int BoardI2cDevBend(void)
{
    x_err_t ret = EOK;
    static struct I2cHardwareDevice i2c_device0;
    memset(&i2c_device0, 0, sizeof(struct I2cHardwareDevice));

    i2c_device0.i2c_dev_done = &i2c_dev_done;

    ret = I2cDeviceRegister(&i2c_device0, NONE, I2C_1_DEVICE_NAME_0);
    if (EOK != ret) {
        i2c_print("BoardI2cDevBend I2cDeviceInit device %s error %d\n", I2C_1_DEVICE_NAME_0, ret);
        return ERROR;
    }

    ret = I2cDeviceAttachToBus(I2C_1_DEVICE_NAME_0, I2C_BUS_NAME_1);
    if (EOK != ret) {
        i2c_print("BoardI2cDevBend I2cDeviceAttachToBus device %s error %d\n", I2C_1_DEVICE_NAME_0, ret);
        return ERROR;
    }

    return ret;
}

/*BOARD I2C INIT*/
int Stm32HwI2cInit(void)
{
    static int init_flag = 0;
    x_err_t ret = EOK;

    if(init_flag)
    {
        return ret;
    }
    init_flag = 1;

    static struct I2cBus i2c_bus;
    memset(&i2c_bus, 0, sizeof(struct I2cBus));
    static struct I2cDriver i2c_driver;
    memset(&i2c_driver, 0, sizeof(struct I2cDriver));

#ifdef  BSP_USING_I2C1
    i2c_driver.configure = I2cDrvConfigure;

    ret = BoardI2cBusInit(&i2c_bus, &i2c_driver);
    if (EOK != ret) {
        i2c_print("board_i2c_Init error ret %u\n", ret);
        return ERROR;
    }

    ret = BoardI2cDevBend();
    if (EOK != ret) {
        i2c_print("board_i2c_Init error ret %u\n", ret);
        return ERROR;
    }
#endif

    return ret;
}