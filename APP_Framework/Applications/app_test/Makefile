include $(KERNEL_ROOT)/.config
ifeq ($(CONFIG_ADD_NUTTX_FETURES),y)
    include $(APPDIR)/Make.defs

    ifeq ($(CONFIG_USER_TEST_SEMC),y)
        CSRCS += test_extsram.c
    endif

    ifeq ($(CONFIG_USER_TEST_LCD),y)
        CSRCS += test_lcd.c
    endif

    ifeq ($(CONFIG_BSP_USING_TOUCH),y)
        CSRCS += test_touch.c
    endif

    ifeq ($(CONFIG_MUSL_LIBC),y)
        CSRCS += test_musl.c
    endif

    include $(APPDIR)/Application.mk

endif

ifeq ($(CONFIG_ADD_XIZI_FETURES),y)
    SRC_FILES := test_shell.c

    ifeq ($(CONFIG_USER_TEST_ADC),y)
        SRC_FILES += test_adc.c
    endif

    ifeq ($(CONFIG_USER_TEST_DAC),y)
        SRC_FILES += test_dac.c
    endif

    ifeq ($(CONFIG_USER_TEST_FS),y)
        SRC_FILES += test_fs.c
    endif

    ifeq ($(CONFIG_USER_TEST_SEMC),y)
        SRC_FILES += test_extsram.c
    endif

    ifeq ($(CONFIG_USER_TEST_LCD),y)
        SRC_FILES +=
    endif

    ifeq ($(CONFIG_USER_TEST_I2C),y)
        SRC_FILES += test_i2c.c
    endif

    ifeq ($(CONFIG_USER_TEST_GPIO),y)
        SRC_FILES += test_gpio.c
        ifeq ($(CONFIG_GPIO_POLL_MODE),y)
            SRC_FILES += polling_gpio.c
        endif
        ifeq ($(CONFIG_GPIO_INTERRUPT_MODE),y)
            SRC_FILES += interrupt_gpio.c
        endif
    endif

    ifeq ($(CONFIG_USER_TEST_LORA),y)
        SRC_FILES += test_loraE220.c
    endif

    ifeq ($(CONFIG_USER_TEST_RTC),y)
        SRC_FILES += test_rtc.c
    endif

    ifeq ($(CONFIG_USER_TEST_RS485),y)
        SRC_FILES += test_rs485.c
    endif    

    ifeq ($(CONFIG_USER_TEST_HWTIMER),y)
        SRC_FILES += test_hwtimer.c
    endif  

    ifeq ($(CONFIG_USER_TEST_LCD_EDU),y)
        SRC_FILES += test_lcd_edu.c
        ifeq ($(CONFIG_CONFIG_LCD_EDU_RTC),y)
            SRC_FILES += my_lcd_with_rtc.c
        endif
        ifeq ($(CONFIG_CONFIG_MY_LCD),y)
            SRC_FILES += my_lcd.c
        endif
    endif

    ifeq ($(CONFIG_USER_TEST_WDT),y)
        SRC_FILES += test_wdt.c
        ifeq ($(CONFIG_CONFIG_MY_WDT),y)
            SRC_FILES += my_wdt.c
        endif
    endif 

    ifeq ($(CONFIG_USER_TEST_TOUCH),y)
        SRC_FILES += test_touch.c
    endif 

    ifeq ($(CONFIG_USER_TEST_CAMERA),y)
        SRC_FILES += test_camera.c
    endif

    ifeq ($(CONFIG_USER_TEST_ETHERNET),y)
        SRC_FILES += test_ethernet.c
    endif

    ifeq ($(CONFIG_USER_TEST_FLASH),y)
        SRC_FILES += test_flash.c
    endif

    ifeq ($(CONFIG_USER_TEST_CAN),y)
        SRC_FILES += test_can.c
    endif    

    include $(KERNEL_ROOT)/compiler.mk
endif
