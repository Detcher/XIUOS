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
#include <transform.h>

#ifdef ADD_XIZI_FETURES
#include <stdio.h>
#include <string.h>
#include <my_board.h>
#include <connect_lcd.h>

void TestMyLcd(void)
{
    int lcd_fd = PrivOpen(EDU_LCD_DEV_DRIVER, O_RDWR);
    if (lcd_fd < 0)
    {
        printf("open lcd fd error:%d\n", lcd_fd);
        return;
    }

    DrvLcdClear(0xFFFF);

    LcdDrawRectangle( 65, 140, 255, 180, 0x0000 );
    LcdShowString( 70, 155, 250, 25, 24, "Prod by. Detcher", 0x0000, 0xFFFF );
    
    PrivClose(lcd_fd);
}

PRIV_SHELL_CMD_FUNCTION(TestMyLcd, a lcd test sample, PRIV_SHELL_CMD_MAIN_ATTR);
#endif