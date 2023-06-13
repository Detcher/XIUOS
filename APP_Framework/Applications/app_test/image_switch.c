/**
 * @author Detcher
 * @date   2023.6.12
 * @brief  suppoort functions:
 *          switch images
 *          
 * @todo   snapshot(BUGs there)
 *         U-disk store/get images
 *         lvgl menu
 *         . . . 
*/
#include <transform.h>
#include <my_board.h>
#include <image/bulb.h>
#include <image/sky.h>
#include <image/mountain.h>
#include <stdio.h>
#include <string.h>

#define IMG_CNT 3

static int IMG_NUM;
static uint8_t *img[IMG_CNT] = {
    bulb_map, mountain_map, sky_map
};

static void GPIO_irqHandler( void *args )
{
    int **arg_vec = (int **)args;
    
    // disable interrupt
    struct PinParam parameter;
    parameter.cmd = GPIO_IRQ_DISABLE;
    parameter.pin = BSP_KEY_PIN;

    struct PrivIoctlCfg ioctl_cfg;
    ioctl_cfg.ioctl_driver_type = PIN_TYPE;
    ioctl_cfg.args =  (void *)&parameter;

    if (0 != PrivIoctl(*arg_vec[1], OPE_CFG, &ioctl_cfg)) {
        printf("ioctl pin fd error %d\n", *arg_vec[1]);
        PrivClose(*arg_vec[1]);
        return;
    }

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
        PrivWrite(*arg_vec[0], &graph_param, NULL_PARAMETER);
    }

    for (int i = 0; i < IMAGE_HEIGHT; i++)
    {
        graph_param.pixel_info.pixel_color = img[IMG_NUM] + i * IMAGE_WIDTH;
        graph_param.pixel_info.x_startpos = 0;
        graph_param.pixel_info.y_startpos = i + (LCD_SIZE - IMAGE_HEIGHT) / 2;
        graph_param.pixel_info.x_endpos = IMAGE_WIDTH - 1;
        graph_param.pixel_info.y_endpos = i + (LCD_SIZE - IMAGE_HEIGHT) / 2;
        PrivWrite(*arg_vec[0], &graph_param, NULL_PARAMETER);
    }

    IMG_NUM = (IMG_NUM + 1) % IMG_CNT;

    // enable interrupt
    parameter.cmd = GPIO_IRQ_ENABLE;
    parameter.pin = BSP_KEY_PIN;

    ioctl_cfg.ioctl_driver_type = PIN_TYPE;
    ioctl_cfg.args =  (void *)&parameter;

    if (0 != PrivIoctl(*arg_vec[1], OPE_CFG, &ioctl_cfg)) {
        printf("ioctl pin fd error %d\n", *arg_vec[1]);
        PrivClose(*arg_vec[1]);
        return;
    }
}

void Image_switcher(void)
{
        // LCD part
    int lcd_fd = PrivOpen(EDU_LCD_DEV_DRIVER, O_RDWR);
    if (lcd_fd < 0)
    {
        printf("open lcd fd error:%d\n", lcd_fd);
        return;
    }

        // Interrupt GPIO part
    int pin_fd = PrivOpen(GPIO_DEV_DRIVER, O_RDWR);
    if(pin_fd<0){
        printf("open pin fd error:%d\n",pin_fd);
        return;
    }

    int *args[2] = { &lcd_fd, &pin_fd };

    //config led pin in board
    struct PinParam parameter;
    parameter.cmd = GPIO_CONFIG_MODE;
    parameter.pin = BSP_LED_PIN;
    parameter.mode = GPIO_CFG_OUTPUT;

    struct PrivIoctlCfg ioctl_cfg;
    ioctl_cfg.ioctl_driver_type = PIN_TYPE;
    ioctl_cfg.args =  (void *)&parameter;

    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }

    // config key pin in board
    parameter.pin = BSP_KEY_PIN;
    parameter.mode = GPIO_CFG_INPUT;

    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
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

    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }

    // enable interrupt for specific pin
    parameter.cmd = GPIO_IRQ_ENABLE;
    parameter.pin = BSP_KEY_PIN;
    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }

    // infinite-loop
    for( ; ; ) {

    }

    PrivClose(lcd_fd);
}

PRIV_SHELL_CMD_FUNCTION(Image_switcher, a lcd test sample, PRIV_SHELL_CMD_MAIN_ATTR);