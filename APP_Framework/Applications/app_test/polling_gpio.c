#include <stdio.h>
#include <string.h>
#include <transform.h>
#include <my_board.h>

void  TestPollGpio(void)
{
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

    struct PrivIoctlCfg ioctl_cfg;
    ioctl_cfg.ioctl_driver_type = PIN_TYPE;
    ioctl_cfg.args =  (void *)&parameter;

    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }

    //config key pin in board
     parameter.pin = BSP_KEY_PIN;
     parameter.mode = GPIO_CFG_INPUT;

    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    } 

    struct PinStat pin_led;
    struct PinStat pin_key;

    pin_led.pin = BSP_LED_PIN;
    pin_key.pin = BSP_KEY_PIN;

    pin_led.val = GPIO_LOW;

    //recycle read pin and write pin until key break
    while(1){
        if(0>PrivRead(pin_fd,&pin_key,NULL_PARAMETER)){
            printf("read pin fd error %d\n", pin_fd);
            PrivClose(pin_fd);
            return;
        }

        // pin_key.val == 0 if key pressed, then halt execution
        if(pin_key.val == 0){
            printf("exit successfully!\n");
            PrivClose(pin_fd);
            return;
        }            
        
        if(0>PrivWrite(pin_fd,&pin_led,NULL_PARAMETER)){
                printf("write pin fd error %d\n", pin_fd);
                PrivClose(pin_fd);
                return;
        }   
        
        // Question here!!! if i set "pin_led.val = GPIO_HIGH;"(line 56) above rather than GPIO_LOW,
        // and use "pin_led.val = ~pin_led.val;", it work! But if i set val to "GPIO_LOW",
        // it won't work. But using "~" is wrong definitely, because ~1 != 0 and ~0 != 1, so we should use "!".
        // And the qeustion is, why the first case works???
        pin_led.val = !pin_led.val;
        PrivTaskDelay(1000);
    }
}

PRIV_SHELL_CMD_FUNCTION(TestPollGpio, a gpio test sample, PRIV_SHELL_CMD_MAIN_ATTR);