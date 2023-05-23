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
* @file:    test_lcd.c
* @brief:   a application of lcd function
* @version: 1.1
* @author:  AIIT XUOS Lab
* @date:    2022/12/17
*/

#include <stdio.h>
#include <string.h>
#include <transform.h>
#include <connect_lcd.h>
#include <color.h>
#include <my_board.h>

#define GRAPHIC_CTRL_RECT_UPDATE 0x00
#define LCD_STRING_TYPE 0
#define LCD_DOT_TYPE 1
#define LCD_FONT_RECT_WIDTH 150
#define LCD_FONT_RECT_HEIGHT 50
#define NULL_PARAMETER 0

void TestLcdWithRTC(void)
{
    // RTC part
    int rtc_fd = PrivOpen(RTC_DEV_DRIVER, O_RDWR);
    if(rtc_fd<0){
        printf("open rtc fd error:%d\n",rtc_fd);
        return;
    }

    struct RtcDrvConfigureParam rtc_para;
    struct tm *ct;
    time_t my_time=0;

    rtc_para.rtc_operation_cmd = OPER_RTC_SET_TIME;
    rtc_para.time = &my_time;

    struct PrivIoctlCfg ioctl_cfg;
    ioctl_cfg.ioctl_driver_type = RTC_TYPE;
    ioctl_cfg.args =  (void *)&rtc_para;

    PrivIoctl(rtc_fd,0,&ioctl_cfg);

    // Lcd part
    int lcd_fd = PrivOpen(EDU_LCD_DEV_DRIVER, O_RDWR);
    if (lcd_fd < 0)
    {
        printf("open lcd fd error:%d\n", lcd_fd);
        return;
    }

    LcdWriteParam graph_param;

    DrvLcdClear(N_Coloe_B16Salmon);
    rtc_para.rtc_operation_cmd = OPER_RTC_GET_TIME;
    while(1) {
        PrivIoctl(rtc_fd,0,&ioctl_cfg);
        ct = localtime(rtc_para.time);
        // draw text
        graph_param.type = LCD_STRING_TYPE;
        graph_param.string_info.x_pos = 0;
        graph_param.string_info.y_pos = 0;
        graph_param.string_info.width = 320;
        graph_param.string_info.height = 40;
        graph_param.string_info.font_size = 24;
        graph_param.string_info.back_color = 0xFFFF;
        graph_param.string_info.font_color = 0x0000;
        graph_param.string_info.addr = (uint8 *)asctime(ct);
        PrivWrite(lcd_fd, &graph_param, NULL_PARAMETER);
        PrivTaskDelay(100);
    }
    
    PrivClose(rtc_fd);
    PrivClose(lcd_fd);
}

PRIV_SHELL_CMD_FUNCTION(TestLcdWithRTC, a lcd test sample, PRIV_SHELL_CMD_MAIN_ATTR);