/*
 * Copyright (c) 2017 Simon Goldschmidt
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Simon Goldschmidt
 *
 */
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
* @file sys_arch.h
* @brief In order to adapt to XiUOS, some changes have been made to implement the LwIP interface.
* @version 1.0
* @author AIIT XUOS Lab
* @date 2021-05-29
*/

#ifndef __SYS_ARCH__
#define __SYS_ATCH__

#include <lwip/opt.h>
#include <lwip/arch.h>
#include <lwip/netif.h>

#include "tcpip.h"

#include <xs_base.h>

/* USER CODE BEGIN 0 */
// #define SET_AS_SERVER 1 /* define this terminal is udp server or not*/

#define LOCAL_PORT_SERVER           4840
#define TARGET_PORT_CLIENT          LOCAL_PORT_SERVER

#define TEST_LWIP_TIMES             3

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0_SERVER             192
#define IP_ADDR1_SERVER             168
#define IP_ADDR2_SERVER             250
#define IP_ADDR3_SERVER             252

#define IP_ADDR0_ClIENT             192
#define IP_ADDR1_ClIENT             168
#define IP_ADDR2_ClIENT             250
#define IP_ADDR3_ClIENT             253

/*NETMASK*/
#define NETMASK_ADDR0               255
#define NETMASK_ADDR1               255
#define NETMASK_ADDR2               255
#define NETMASK_ADDR3                 0

/*Gateway Address*/
#define GW_ADDR0                    192
#define GW_ADDR1                    168
#define GW_ADDR2                    250
#define GW_ADDR3                    5

/* MAC address configuration. */
#define configMAC_ADDR { 0x02, 0x12, 0x13, 0x10, 0x15, 0x11}

/* USER CODE END 0 */

#define SYS_MBOX_NULL  -1
#define SYS_SEM_NULL   0
#define SYS_MRTEX_NULL SYS_SEM_NULL

typedef int32 sys_sem_t;
typedef int32 sys_mutex_t;
typedef int32 sys_mbox_t;
typedef int32 sys_thread_t;

typedef x_base sys_prot_t;

#define MS_PER_SYSTICK_F407 1000/TICK_PER_SECOND

extern char lwip_ipaddr[];
extern char lwip_netmask[];
extern char lwip_gwaddr[];
extern int is_lwip_test;
extern struct netif gnetif;

void TcpIpInit(void);
void lwip_config_net(char *ip, char *mask, char *gw);
void lwip_config_tcp(char *ip, char *mask, char *gw);

#endif
