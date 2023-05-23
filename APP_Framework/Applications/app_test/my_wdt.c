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
* @file:    test_wdt.c
* @brief:   a application of wdt function
* @version: 1.1
* @author:  AIIT XUOS Lab
* @date:    2022/12/17
*/

/**
 * @problems: for those have-opened fd like rtc_fd, pin_fd... if watchdog resets the whole system, will them be closed because of the reset or
 *            still remain opening state?? Also, will the interrupts set before the reset remain still or clear the whole interrupt vector table? (i guess the former.
*/
#include <transform.h>

#ifdef ADD_XIZI_FETURES
#include <stdio.h>
#include <string.h>
#include <my_board.h>

static void GPIO_irqHandler( void *args ) {
    int **p_rtc_fd_flag = (int **)args;
    *p_rtc_fd_flag[1] = 1;

    struct RtcDrvConfigureParam rtc_para;
    struct tm *ct;
    time_t my_time=0;

    rtc_para.rtc_operation_cmd = OPER_RTC_SET_TIME;
    rtc_para.time = &my_time;

    struct PrivIoctlCfg ioctl_cfg;
    ioctl_cfg.ioctl_driver_type = RTC_TYPE;
    ioctl_cfg.args =  (void *)&rtc_para;

    PrivIoctl(*p_rtc_fd_flag[0],0,&ioctl_cfg);
}

void TestMyWDT(int argc, char *agrv[])
{
    // pass to irq, similar to "char *argv[]".
    int *args[2];
    
    // RTC part.
    int rtc_fd = PrivOpen(RTC_DEV_DRIVER, O_RDWR);
    args[0] = &rtc_fd;

    if(rtc_fd<0){
        printf("open rtc fd error:%d\n",rtc_fd);
        return;
    }

    struct RtcDrvConfigureParam rtc_para;
    struct tm *ct;
    time_t my_time=0;

    rtc_para.rtc_operation_cmd = OPER_RTC_SET_TIME;
    rtc_para.time = &my_time;

    struct PrivIoctlCfg ioctl_cfg_0;
    ioctl_cfg_0.ioctl_driver_type = RTC_TYPE;
    ioctl_cfg_0.args =  (void *)&rtc_para;

    PrivIoctl(rtc_fd,0,&ioctl_cfg_0);
    
    // gpio interrupt part.
    int flag = 0;
    args[1] = &flag;

    int pin_fd = PrivOpen(GPIO_DEV_DRIVER, O_RDWR);
    if(pin_fd<0){
        printf("open pin fd error:%d\n",pin_fd);
        return;
    }

        //config led pin in board
    struct PinParam parameter;
    parameter.cmd = GPIO_CONFIG_MODE;
    parameter.pin = BSP_LED_PIN;
    parameter.mode = GPIO_CFG_OUTPUT;

    struct PrivIoctlCfg ioctl_cfg_1;
    ioctl_cfg_1.ioctl_driver_type = PIN_TYPE;
    ioctl_cfg_1.args =  (void *)&parameter;

    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg_1)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }

        // config key pin in board
    parameter.pin = BSP_KEY_PIN;
    parameter.mode = GPIO_CFG_INPUT;

    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg_1)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    } 

        // config key pin interrupt-related in board
    parameter.cmd = GPIO_IRQ_REGISTER;
    parameter.pin = BSP_KEY_PIN;
    parameter.irq_set.irq_mode = GPIO_IRQ_EDGE_FALLING;
    parameter.irq_set.hdr = GPIO_irqHandler;
    parameter.irq_set.args = (void *)args;

    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg_1)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }

        // enable interrupt for specific pin
    parameter.cmd = GPIO_IRQ_ENABLE;
    parameter.pin = BSP_KEY_PIN;
    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg_1)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }
    
    // watchdog part.
    int wdt_fd = PrivOpen(WDT0_DEV_DRIVER, O_RDWR);
    if (wdt_fd < 0)
    {
        printf("open wdt_fd fd error:%d\n", wdt_fd);
        return;
    }
    printf("hw watchdog open!\n");

        // init watchdog
    int wdt_time = 10000;

    struct PrivIoctlCfg ioctl_cfg_2;
    ioctl_cfg_2.ioctl_driver_type = WDT_TYPE;
    ioctl_cfg_2.args = (void *)&wdt_time;

    if (0 != PrivIoctl(wdt_fd, OPER_WDT_SET_TIMEOUT, &ioctl_cfg_2))
    {
        printf("ioctl wdt fd error %d\n", wdt_fd);
        PrivClose(wdt_fd);
        return;
    }

    // main function.
    rtc_para.rtc_operation_cmd = OPER_RTC_GET_TIME;
    while(1) {
        if( flag == 1 ) {
            flag = 0;
            printf("dog is feed\n");
            PrivIoctl(wdt_fd, OPER_WDT_KEEPALIVE, &ioctl_cfg_2); // feed dog
        }

        PrivIoctl(rtc_fd,0,&ioctl_cfg_0);
        ct = localtime(rtc_para.time);
        printf("%d\n", ct->tm_sec);

        PrivTaskDelay(500);
    }

    PrivClose(pin_fd);
    PrivClose(wdt_fd);
    PrivClose(rtc_fd);
    return;
}

PRIV_SHELL_CMD_FUNCTION(TestMyWDT, a wdt test sample, PRIV_SHELL_CMD_MAIN_ATTR);
#endif