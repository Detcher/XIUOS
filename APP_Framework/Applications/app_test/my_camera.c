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
* @file:    my_camera.c
* @brief:   based on the camera function, aiming to fulfill the "snapshot" function, but still has 
*           bunches of BUG :(
* @version: 0.1
* @author:  Detcher
* @date:    2023/6/11
*/

#include <transform.h>
#include <my_board.h>
#include <stdio.h>
#include <string.h>
#include <dvp.h>

#define BUFF_SIZE 384000
// #define STORE_PATH "/IMAGE"

// static int fd;
static uint16_t image_buff[BUFF_SIZE];
static int pin_fd;
// static uint16_t screen_shot[BUFF_SIZE];

static void GPIO_irqHandler( void *args ) {
    // struct PinParam parameter;
    // parameter.cmd = GPIO_IRQ_DISABLE;
    // parameter.pin = BSP_KEY_PIN;

    // struct PrivIoctlCfg ioctl_cfg;
    // ioctl_cfg.ioctl_driver_type = PIN_TYPE;
    // ioctl_cfg.args =  (void *)&parameter;

    // if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
    //     printf("ioctl pin fd error %d\n", pin_fd);
    //     PrivClose(pin_fd);
    //     return;
    // }
    
    // dvp_set_output_enable( 0, 0 );
    int **arg_vec = (int **)args;
    *arg_vec[0] = 1;
    // lvgl animation(ka-cha) TO-DO

    // store the current image_buff[]
    // strncpy( (char *)screen_shot, (char *)image_buff, BUFF_SIZE ); 
    printf("irqHandler: screen-shot successfully!\n");

    LcdWriteParam graph_param;
    graph_param.type = LCD_DOT_TYPE;

    for (int i = 0; i < IMAGE_HEIGHT; i++)
    {
        graph_param.pixel_info.pixel_color = image_buff + i * IMAGE_WIDTH;
        graph_param.pixel_info.x_startpos = 0;
        graph_param.pixel_info.y_startpos = i + (LCD_SIZE - IMAGE_HEIGHT) / 2;
        graph_param.pixel_info.x_endpos = IMAGE_WIDTH - 1;
        graph_param.pixel_info.y_endpos = i + (LCD_SIZE - IMAGE_HEIGHT) / 2;
        PrivWrite(*arg_vec[1], &graph_param, NULL_PARAMETER);
    }

    // int err_flag = write(fd,image_buff,strlen((char *)image_buff));
    // if(err_flag<0){
    //     printf("write failed,error:%d\n",err_flag);
    //     return;
    // }
    // dvp_set_output_enable( 0, 1 );
    // parameter.cmd = GPIO_IRQ_ENABLE;
    // parameter.pin = BSP_KEY_PIN;

    // ioctl_cfg.ioctl_driver_type = PIN_TYPE;
    // ioctl_cfg.args =  (void *)&parameter;

    // if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
    //     printf("ioctl pin fd error %d\n", pin_fd);
    //     PrivClose(pin_fd);
    //     return;
    // }
}

void TestMyCamera(int argc, char *argv[])
{      
    // fd = open( FPATH, O_RDWR | O_CREAT );
    // if(fd<0){
    //     printf("fs fd open error:%d\n",fd);
    //     return;
    // }
    // printf("open file success:)\n");

    int flag = 0;    
        // LCD part
    int lcd_fd = PrivOpen(CAMERA_LCD_DEV_DRIVER, O_RDWR);
    if (lcd_fd < 0)
    {
        printf("open lcd fd error:%d\n", lcd_fd);
        return;
    }
        
    int *args[2] = { &flag, &lcd_fd };

        // Interrupt gpio part
    int pin_fd = PrivOpen(GPIO_DEV_DRIVER, O_RDWR);
    if(pin_fd<0){
        printf("open pin fd error:%d\n",pin_fd);
        return;
    }

    // config led pin in board
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
    
    printf("Interrupt gpio register over:)\n");
    
        // Camera part
    int frame_counter = 10000;
    if (argc > 1)
    {
        frame_counter = atoi(argv[1]);
    }
    printf("This test will refresh %d frames, unless u interrupt it:)\n", frame_counter);

    int camera_fd = PrivOpen(CAMERA_DEV_DRIVER, O_RDWR);
    if (camera_fd < 0)
    {
        printf("open camera fd error:%d\n", camera_fd);
        return;
    }

    //configure the camera's output address
    struct PrivIoctlCfg ioctl_cfg;
    ioctl_cfg.ioctl_driver_type = CAMERA_TYPE;
    struct CameraCfg camera_cfg ={
        .gain_manu_enable = 0,
        .gain = 0xFF,
        .window_w = 800,
        .window_h = 600,
        .output_w = IMAGE_WIDTH,
        .output_h = IMAGE_HEIGHT,
        .window_xoffset = 0,
        .window_yoffset = 0
    };
    ioctl_cfg.args = &camera_cfg;
    if (0 != PrivIoctl(camera_fd, OPE_CFG, &ioctl_cfg))
    {
        printf("camera pin fd error %d\n", camera_fd);
        PrivClose(camera_fd);
        return;
    }

    ioctl_cfg.args = (void *)image_buff;

    if (0 != PrivRead(camera_fd, image_buff, NULL_PARAMETER))
    {
        printf("camera pin fd error %d\n", camera_fd);
        PrivClose(camera_fd);
        return;
    }

    printf("address buff is %x\n", image_buff);


    LcdWriteParam graph_param;
    graph_param.type = LCD_DOT_TYPE;

    //clear the LCD
    uint16_t back_color[LCD_SIZE];
    memset(back_color,0,sizeof(back_color));
    for (int i = 0; i < LCD_SIZE; i++)
    {
        graph_param.pixel_info.pixel_color = &back_color;
        graph_param.pixel_info.x_startpos = 0;
        graph_param.pixel_info.y_startpos = i;
        graph_param.pixel_info.x_endpos = LCD_SIZE -1;
        graph_param.pixel_info.y_endpos = i;
        PrivWrite(lcd_fd, &graph_param, NULL_PARAMETER);
    }

    //refresh the LCD using photo of camera
    while (frame_counter--)
    {
        if( flag == 1 ) {
            break;
        }
        // disable interrupt for a moment
        parameter.cmd = GPIO_IRQ_DISABLE;
        parameter.pin = BSP_KEY_PIN;
        if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg_1)) {
            printf("ioctl pin fd error %d\n", pin_fd);
            PrivClose(pin_fd);
            return;
        }

        // refresh with a new frame 
        for (int i = 0; i < IMAGE_HEIGHT; i++)
        {
            graph_param.pixel_info.pixel_color = image_buff + i * IMAGE_WIDTH;
            graph_param.pixel_info.x_startpos = 0;
            graph_param.pixel_info.y_startpos = i + (LCD_SIZE - IMAGE_HEIGHT) / 2;
            graph_param.pixel_info.x_endpos = IMAGE_WIDTH - 1;
            graph_param.pixel_info.y_endpos = i + (LCD_SIZE - IMAGE_HEIGHT) / 2;
            PrivWrite(lcd_fd, &graph_param, NULL_PARAMETER);
        }

        // enable interrupt again
        parameter.cmd = GPIO_IRQ_ENABLE;
        parameter.pin = BSP_KEY_PIN;
        if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg_1)) {
            printf("ioctl pin fd error %d\n", pin_fd);
            PrivClose(pin_fd);
            return;
        }
    }

    parameter.cmd = GPIO_IRQ_FREE;
    parameter.pin = BSP_KEY_PIN;
    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg_1)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }
    // close test
    PrivClose(lcd_fd);
    PrivClose(camera_fd);
    PrivClose(pin_fd);
    // close(fd);
    printf("The camera test is finished successfully\n");
}

PRIV_SHELL_CMD_FUNCTION(TestMyCamera, a camera test sample, PRIV_SHELL_CMD_MAIN_ATTR);