#include <stdio.h>
#include <string.h>
#include <transform.h>
#include <my_board.h>

static void GPIO_irqHandler( void *args ) {
    int *p_flag = (int *)args;
    *p_flag = 1;

    int pin_fd = PrivOpen(GPIO_DEV_DRIVER, O_RDWR);
    if(pin_fd<0){
        printf("open pin fd error:%d\n",pin_fd);
        return;
    }

    // disable && free interrupt on the specific pin
    struct PinParam parameter;
    parameter.cmd = GPIO_IRQ_DISABLE;
    parameter.pin = BSP_KEY_PIN;

    struct PrivIoctlCfg ioctl_cfg;
    ioctl_cfg.ioctl_driver_type = PIN_TYPE;
    ioctl_cfg.args =  (void *)&parameter;

    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }

    parameter.cmd = GPIO_IRQ_FREE;
    parameter.pin = BSP_KEY_PIN;
    if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
        printf("ioctl pin fd error %d\n", pin_fd);
        PrivClose(pin_fd);
        return;
    }

    PrivClose(pin_fd);
    printf("exit successfully!\n");
}

void  TestInterruptGpio(void)
{
    int flag = 0;
    // KPrintf("test_1\n");
    // KPrintf("%d\t%d\n", flag, &flag);
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
    parameter.irq_set.args = (void *)&flag;

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

    struct PinStat pin_led;
    pin_led.pin = BSP_LED_PIN;
    pin_led.val = GPIO_HIGH;

    //recycle read pin and write pin until key break
    while(1){        
        if(0>PrivWrite(pin_fd,&pin_led,NULL_PARAMETER)){
                printf("write pin fd error %d\n", pin_fd);
                PrivClose(pin_fd);
                return;
         }           

        pin_led.val = !pin_led.val;
        PrivTaskDelay(1000);

        // KPrintf("Inside while: %d\n", flag);
        if( flag == 1 ) {
            PrivClose(pin_fd);
            break;
        }
    }
}

PRIV_SHELL_CMD_FUNCTION(TestInterruptGpio, a gpio test sample, PRIV_SHELL_CMD_MAIN_ATTR);

// // // falied-History
// // /*
// // #include <stdio.h>
// // #include <string.h>
// // #include <transform.h>
// // #include <sysctl.h>
// // #include <gpiohs.h>
// // #include <fpioa.h>
// // #include <plic.h>

// // #define BSP_LED_PIN 29
// // #define BSP_KEY_PIN 31
// // #define NULL_PARAMETER 0

// // static uint32_t irq_flag;

// // // #define GPIO_IRQ_EDGE_FALLING            0x01
// // // #define GPIO_IRQ_REGISTER                0xfffffffe
// // // #define GPIO_IRQ_ENABLE                  0xfffffffb


// // static int GPIO_irqHandler(void *ctx) {
// //     irq_flag = gpiohs_get_pin(5);
// //     KPrintf("IRQ The PIN is %d\n", irq_flag);
// //     return 0;
// // }

// // void  TestGpio(void)
// // {
    
// //     // struct PinDevIrq foo;
// //     // foo.irq_mode = GPIO_IRQ_EDGE_FALLING;
// //     // foo.hdr = GPIO_irqHandler;
// //     // foo.args = NULL;

// //     //config led pin in board
     

// //     //config key pin in board
// //     //  parameter.cmd = GPIO_IRQ_REGISTER;
     
// //     //  parameter.irq_set = foo;


// //     // parameter.cmd = GPIO_IRQ_ENABLE;
// //     // if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
// //     //     printf("ioctl pin fd error %d\n", pin_fd);
// //     //     PrivClose(pin_fd);
// //     //     return;
// //     // }
// //     // fpioa_set_function_raw( BSP_LED_PIN, FUNC_GPIOHS1 );
// //     // gpiohs_set_drive_mode( 1, GPIO_DM_OUTPUT );

// //     plic_init();
// //     sysctl_enable_irq();

// //     FpioaSetFunction( BSP_LED_PIN, FUNC_GPIOHS3 );
// //     gpiohs_set_drive_mode( 3, GPIO_DM_OUTPUT );
// //     gpio_pin_value_t value = GPIO_PV_HIGH;
// //     gpiohs_set_pin( 3, value );

// //     FpioaSetFunction( BSP_KEY_PIN, FUNC_GPIOHS5 );
// //     gpiohs_set_drive_mode( 5, GPIO_DM_INPUT_PULL_UP );
// //     gpiohs_set_pin_edge( 5, GPIO_PE_FALLING );
// //     // gpiohs_set_irq( 5, 1, GPIO_irqHandler );
// //     gpiohs_irq_register( 5, 1, GPIO_irqHandler, NULL );

// //     //recycle read pin and write pin until key break
// //     while(1){        
// //         gpiohs_set_pin( 3, value = !value );
// //         PrivTaskDelay(1000);
// //     }
// //     gpiohs_irq_unregister(5);
// // }

// // PRIV_SHELL_CMD_FUNCTION(TestGpio, a gpio test sample, PRIV_SHELL_CMD_MAIN_ATTR);
// // */

// #include <stdio.h>
// #include <string.h>
// #include <transform.h>

// #define BSP_LED_PIN 29
// #define BSP_KEY_PIN 31
// #define NULL_PARAMETER 0
//     int flag = 1;
// void stopLed(void *no)
// {
//     flag=1-flag;
//     return ;
// }

// void TestInterruptGpio(void)
// {
//     int changer = 1;
//     int pin_fd = PrivOpen(GPIO_DEV_DRIVER, O_RDWR);
//     int led_fd = PrivOpen(GPIO_DEV_DRIVER, O_RDWR);
//     if (led_fd < 0)
//     {
//         printf("open led fd error:%d\n", led_fd);
//         return;
//     }

//     // config led pin in board
//     struct PinParam parameter;
//     parameter.cmd = GPIO_CONFIG_MODE;
//     parameter.pin = BSP_LED_PIN;
//     parameter.mode = GPIO_CFG_OUTPUT;

//     struct PrivIoctlCfg ioctl_cfg1;
//     ioctl_cfg1.ioctl_driver_type = PIN_TYPE;
//     ioctl_cfg1.args = (void *)&parameter;

//     if (0 != PrivIoctl(led_fd, OPE_CFG, &ioctl_cfg1))
//     {
//         printf("ioctl pin fd error %d\n", led_fd);
//         PrivClose(led_fd);
//         return;
//     }


// // config key pin in board

//     parameter.cmd = GPIO_CONFIG_MODE;
//     parameter.pin = BSP_KEY_PIN;
//     parameter.mode = GPIO_CFG_INPUT;

//     if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg1))
//     {
//         printf("ioctl pin fd error %d\n", pin_fd);
//         PrivClose(pin_fd);
//         return;
//     }



//     int32 id =KTaskCreate("stopLed",stopLed,NULL,4096,25);

//     // config irq in board
//     parameter.cmd = GPIO_IRQ_REGISTER;
//     parameter.pin = BSP_KEY_PIN;
//    // parameter.mode = GPIO_CFG_INPUT_PULLUP;
//    // parameter.irq_set.args=NULL;
//     parameter.irq_set.hdr= (void *)&stopLed;
//     parameter.irq_set.irq_mode=GPIO_IRQ_EDGE_FALLING;
//     parameter.irq_set.args=(void *)&id;


//     if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg1))
//     {
//         printf("ioctl pin fd error %d\n", pin_fd);
//         PrivClose(pin_fd);
//         return;
//     }
//     parameter.cmd=GPIO_IRQ_ENABLE;
//     parameter.pin=BSP_KEY_PIN;
    
//     if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg1))
//     {
//         printf("ioctl pin fd error %d\n", pin_fd);
//         PrivClose(pin_fd);
//         return;
//     }

//     struct PinStat pin_led;
//     struct PinStat pin_key;

//     pin_led.pin = BSP_LED_PIN;
//     pin_key.pin = BSP_KEY_PIN;
     
//     //StartupKTask(id);
//     // recycle read pin and write pin until key break
//     while (1)
//     {
//         PrivTaskDelay(500);
//         if (flag)
//         {
//             if (changer)
//                 pin_led.val = GPIO_HIGH;
//             else
//                 pin_led.val = GPIO_LOW;
//             changer = 1 - changer;

//             if (0 > PrivWrite(pin_fd, &pin_led, NULL_PARAMETER))
//             {
//                 printf("write pin fd error %d\n", pin_fd);
//                 PrivClose(pin_fd);
//                 return;
//             }
//         }
//     }
// }

// PRIV_SHELL_CMD_FUNCTION(TestInterruptGpio, a gpio test sample, PRIV_SHELL_CMD_MAIN_ATTR);

// #include <stdio.h>
// #include <string.h>
// #include <transform.h>

// static void PinIsr_UserKeyRising( void *args )
// {
//     printf("User Key Pressed!\n");
//     int pin_fd = PrivOpen( GPIO_DEV_DRIVER, O_RDWR );
//     struct PinStat pin_led;
//     pin_led.pin = BSP_LED_PIN;
//     pin_led.val = GPIO_HIGH;
//     if(0 > PrivWrite(pin_fd,&pin_led,NULL_PARAMETER)){
//         printf("write pin fd error %d\n", pin_fd);
//         PrivClose(pin_fd);
//         return;
//     } 

//     printf("Disable Key irq!\n");
//     struct PinParam parameter;
//     struct PrivIoctlCfg ioctl_cfg;
//     ioctl_cfg.ioctl_driver_type = PIN_TYPE;
//     ioctl_cfg.args =  (void *)&parameter;
//     parameter.cmd = GPIO_IRQ_DISABLE;
//     parameter.pin = BSP_KEY_PIN;
//     if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
//         printf("ioctl pin fd error %d\n", pin_fd);
//         PrivClose(pin_fd);
//         return;
//     }

//     PrivClose(pin_fd);

//     printf("exit LedFlip task\n");
//     int32 taskid = *(int32 *)args;
//     KTaskDelete(taskid);
// }

// void LedFlip( void *args )
// {
//     int pin_fd = PrivOpen(GPIO_DEV_DRIVER, O_RDWR);
//     if(pin_fd<0){
//         printf("open pin fd error:%d\n",pin_fd);
//         return;
//     }

//     struct PinStat pin_led;
//     pin_led.pin = BSP_LED_PIN;
//     pin_led.val = GPIO_LOW;
//     while(1){        
//         pin_led.val = !pin_led.val;
//         if(0>PrivWrite(pin_fd,&pin_led,NULL_PARAMETER)){
//                 printf("write pin fd error %d\n", pin_fd);
//                 PrivClose(pin_fd);
//                 return;
//          }           
//         PrivTaskDelay(500);
//     }
// }

// void  TestInterruptGpio(void)
// {
//     int pin_fd = PrivOpen(GPIO_DEV_DRIVER, O_RDWR);
//     if(pin_fd<0){
//         printf("open pin fd error:%d\n",pin_fd);
//         return;
//     }

//     //config led pin in board
//      struct PinParam parameter;
//      parameter.cmd = GPIO_CONFIG_MODE;
//      parameter.pin = BSP_LED_PIN;
//      parameter.mode = GPIO_CFG_OUTPUT;

//     struct PrivIoctlCfg ioctl_cfg;
//     ioctl_cfg.ioctl_driver_type = PIN_TYPE;
//     ioctl_cfg.args =  (void *)&parameter;

//     if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
//         printf("ioctl pin fd error %d\n", pin_fd);
//         PrivClose(pin_fd);
//         return;
//     }

//     // config key pin in board
//     parameter.pin = BSP_KEY_PIN;
//     parameter.mode = GPIO_CFG_INPUT;

//     if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
//         printf("ioctl pin fd error %d\n", pin_fd);
//         PrivClose(pin_fd);
//         return;
//     } 

//     int32 taskid = KTaskCreate( "LedFlip", LedFlip, NULL, 4096, 25 );
//     // config key pin interrupt-related in board
//     parameter.cmd = GPIO_IRQ_REGISTER;
//     parameter.pin = BSP_KEY_PIN;
//     parameter.irq_set.irq_mode = GPIO_IRQ_EDGE_FALLING;
//     parameter.irq_set.hdr = PinIsr_UserKeyRising;
//     parameter.irq_set.args = (void *)&taskid;
//     if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
//         printf("ioctl pin fd error %d\n", pin_fd);
//         PrivClose(pin_fd);
//         return;
//     }

//     // enable interrupt for specific pin
//     parameter.cmd = GPIO_IRQ_ENABLE;
//     parameter.pin = BSP_KEY_PIN;
//     if (0 != PrivIoctl(pin_fd, OPE_CFG, &ioctl_cfg)) {
//         printf("ioctl pin fd error %d\n", pin_fd);
//         PrivClose(pin_fd);
//         return;
//     }

//     StartupKTask(taskid);

//     //recycle read pin and write pin until key break
//     while(1){        
//         PrivTaskDelay(1000);
//         if( KTaskSearch("LedFlip") == 0 ) {
//             break;
//         }
//     }
// }

// PRIV_SHELL_CMD_FUNCTION(TestInterruptGpio, a gpio test sample, PRIV_SHELL_CMD_MAIN_ATTR);