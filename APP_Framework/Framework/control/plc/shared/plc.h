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
 * @file plc.h
 * @brief plc relative definition and structure
 * @version 1.0
 * @author AIIT XUOS Lab
 * @date 2022-01-24
 */

#ifndef __PLC_H_
#define __PLC_H_

#include "bus.h"
#include "xs_klist.h"

#define IP_ADDR_SIZE 32
#define PLC_NAME_SIZE 32

// PLC device information
struct PlcInfo {
    uint32_t ability;      // PLC ability
    uint32_t id;           // PLC Device ID
    uint32_t soft_version; // software version
    uint32_t hard_version; // hardware version
    uint32_t date;         // manufact date
    const char *vendor;    // vendor
    const char *model;     // product model
};

enum PlcSerialType {
    PLC_SERIAL_232,
    PLC_SERIAL_485,
    PLC_SERIAL_CAN
};

union PlcCfg {
    struct {
        char ip_addr[IP_ADDR_SIZE];
        uint32_t ip_port;
    } PlcIpCfg;
    struct {
        enum PlcSerialType serial_type;
        char station_id;
        char serial_port;
    } PlcSerialCfg;
};

struct PlcDevice;

#undef open
#undef close
#undef read
#undef write

// operation API
struct PlcOps {
   int (*open)(void *dev); // open and connect PLC device
   void (*close)(void* dev); // close and disconnect PLC device
   int (*read)(void* dev, void *buf, size_t len); // read data from PLC
   int (*write)(void* dev, const void *buf, size_t len); // write data from PLC
   int (*ioctl)(void* dev, int cmd, void *arg); // send control command to PLC
};

enum PlcCtlType {
    PLC_CTRL_TYPE_HSC,
    PLC_CTRL_TYPE_PID,
    PLC_CTRL_TYPE_PHASING
};


#define PLC_ABILITY_HSC     ((uint32_t)(1 << PLC_CTRL_TYPE_HSC))
#define PLC_ABILITY_PID     ((uint32_t)(1 << PLC_CTRL_TYPE_PID))
#define PLC_ABILITY_PHASING ((uint32_t)(1 << PLC_CTRL_TYPE_PHASING))


enum PlcIndHybridNet
{
    // PLC Field Bus
    PLC_IND_FIELD_MODBUS_485,
    PLC_IND_FIELD_PROFIBUS,
    PLC_IND_FIELD_CANOPEN,
    PLC_IND_FIELD_DEVICENET,
    PLC_IND_FIELD_CONTROLNET,

    // PLC ETHERNET
    PLC_IND_ENET_ETHERNET_IP,
    PLC_IND_ENET_PROFINET,
    PLC_IND_ENET_ETHERCAT,
    PLC_IND_ENET_SERCOS,
    PLC_IND_ENET_OPCUA,

    // PLC wireless net
    PLC_IND_WIRELESS
};

enum PlcTransType
{
    PLC_TRANS_TCP,
    PLC_TRANS_UDP,
    PLC_TRANS_SERIAL
};

//communication interface
struct PlcInterface
{
    char ip_addr[IP_ADDR_SIZE];
    char attrib;
};

// identify PLC device
struct PlcDevice {
    char name[PLC_NAME_SIZE]; /* name of the device */
    enum PlcCtlType type; /* PLC Control Type */
    enum DevState state;
    enum PlcIndHybridNet net;
    enum PlcTransType trans;

    struct PlcInfo info;/* Plc info, such as vendor name and model name */
    union PlcCfg cfg;
    struct PlcOps *ops; /* filesystem-like APIs for data transferring */
    struct PlcInterface interface; /* protocols used for transferring data from program to plc */

    void *priv_data;
    DoubleLinklistType link;/* link list node */
};

int PlcDevRegister(struct PlcDevice *plc_device, void *plc_param, const char *device_name);

#endif
