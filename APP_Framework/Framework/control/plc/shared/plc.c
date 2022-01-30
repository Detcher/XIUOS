/*
* Copyright (c) 2021 AIIT XUOS Lab
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
 * @file plc.c
 * @brief plc relative activities
 * @version 1.0
 * @author AIIT XUOS Lab
 * @date 2021.12.15
 */


#include "open62541.h"
#include "ua_api.h"
#include "plc.h"
#include "plc_bus.h"
#include "plc_dev.h"

#define PLC_BUS_NAME "plc bus"
#define PLC_DRV_NAME "plc driver"

struct PlcDevice plc_device;
struct PlcBus plc_bus;
struct PlcDriver plc_drv;


static DoubleLinklistType plcdev_list;

int PlcDevConfigure(struct HardwareDev *dev, uint8 plc_chip_select, uint8 plc_cs_release);


/*Create the plc device linklist*/
static void PlcDeviceLinkInit()
{
    InitDoubleLinkList(&plcdev_list);
}

static int PlcDeviceOpen(void *dev)
{
    NULL_PARAM_CHECK(dev);

    struct PlcDevice *plc_dev = (struct PlcDevice *)dev;

    if(plc_dev->net == PLC_IND_ENET_OPCUA)
    {
        return ua_open(dev);
    }

    return EOK;
}

static void PlcDeviceClose(void *dev)
{
    NULL_PARAM_CHECK(dev);

    struct PlcDevice *plc_dev = (struct PlcDevice *)dev;

    if(plc_dev->net == PLC_IND_ENET_OPCUA)
    {
        ua_close(dev);
    }
}

static int PlcDeviceWrite(void *dev, const void *buf, size_t len)
{
    NULL_PARAM_CHECK(dev);
    NULL_PARAM_CHECK(write_param);

    int ret;
    struct PlcDevice *plc_dev = (struct PlcDevice *)dev;

    if(plc_dev->net == PLC_IND_ENET_OPCUA)
    {
        ret = ua_write(dev, buf, len);
    }

    return ret;
}

static int PlcDeviceRead(void *dev, void *buf, size_t len)
{
    NULL_PARAM_CHECK(dev);
    NULL_PARAM_CHECK(read_param);

    int ret;
    struct PlcDevice *plc_dev = (struct PlcDevice *)dev;

    if(plc_dev->net == PLC_IND_ENET_OPCUA)
    {
        ret = ua_read(dev, buf, len);
    }

    return ret;
}

static const struct PlcOps plc_done =
{
    .open = PlcDeviceOpen,
    .close = PlcDeviceClose,
    .write = PlcDeviceWrite,
    .read = PlcDeviceRead,
};

struct PlcDevice *PlcDevFind(const char *dev_name, enum DevType dev_type)
{
    NULL_PARAM_CHECK(dev_name);

    struct PlcDevice *device = NONE;

    DoubleLinklistType *node = NONE;
    DoubleLinklistType *head = &plcdev_list;

    for (node = head->node_next; node != head; node = node->node_next) {
        device = SYS_DOUBLE_LINKLIST_ENTRY(node, struct PlcDevice, link);
        if ((!strcmp(device->name, dev_name)) && (dev_type == device->type)) {
            return device;
        }
    }

    KPrintf("PlcDevFind cannot find the %s device.return NULL\n", dev_name);
    return NONE;
}

int PlcDevRegister(struct PlcDevice *plc_device, void *plc_param, const char *device_name)
{
    NULL_PARAM_CHECK(plc_device);
    NULL_PARAM_CHECK(device_name);

    x_err_t ret = EOK;
    static x_bool dev_link_flag = RET_FALSE;

    if (!dev_link_flag) {
        PlcDeviceLinkInit();
        dev_link_flag = RET_TRUE;
    }

    if (DEV_INSTALL != plc_device->state) {
        strncpy(plc_device->name, device_name, strlen(device_name));
        DoubleLinkListInsertNodeAfter(&plcdev_list, &(plc_device->link));
        plc_device->state = DEV_INSTALL;
    } else {
        KPrintf("PlcDevRegister device has been register state%u\n", plc_device->type);
    }

    return ret;
}

int PlcDeviceAttachToBus(const char *dev_name, const char *bus_name)
{
    NULL_PARAM_CHECK(dev_name);
    NULL_PARAM_CHECK(bus_name);

    x_err_t ret = EOK;

    struct Bus *bus;
    struct HardwareDev *device;

    bus = BusFind(bus_name);
    if (NONE == bus) {
        KPrintf("PlcDeviceAttachToBus find plc bus error!name %s\n", bus_name);
        return ERROR;
    }

    if (TYPE_PLC_BUS == bus->bus_type) {
        device = PlcHardwareDevFind(dev_name, TYPE_PLC_DEV);
        if (NONE == device) {
            KPrintf("PlcDeviceAttachToBus find plc device error!name %s\n", dev_name);
            return ERROR;
        }

        if (TYPE_PLC_DEV == device->dev_type) {
            ret = DeviceRegisterToBus(bus, device);
            if (EOK != ret) {
                KPrintf("PlcDeviceAttachToBus DeviceRegisterToBus error %u\n", ret);
                return ERROR;
            }
        }
    }

    return EOK;
}

void PlcTestInit(void)
{
    PlcBusInit(&plc_bus, PLC_BUS_NAME);
    PlcDriverInit(&plc_drv, PLC_DRV_NAME);
}

void test_plc_bus(int argc, char *argv[])
{
    PlcTestInit();
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN) | SHELL_CMD_PARAM_NUM(3),
     plc, test_plc_bus, test PLC);

