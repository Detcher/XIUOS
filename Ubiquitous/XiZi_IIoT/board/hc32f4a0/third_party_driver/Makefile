SRC_DIR += common

ifeq ($(CONFIG_BSP_USING_UART),y)
  SRC_DIR += usart
endif

ifeq ($(CONFIG_BSP_USING_ADC),y)
  SRC_DIR += adc
endif

ifeq ($(CONFIG_BSP_USING_DAC),y)
  SRC_DIR += dac
endif

ifeq ($(CONFIG_BSP_USING_GPIO),y)
  SRC_DIR += gpio
endif

ifeq ($(CONFIG_BSP_USING_LWIP),y)
  SRC_DIR += ethernet
endif

ifeq ($(CONFIG_BSP_USING_SPI),y)
  SRC_DIR += spi
endif

ifeq ($(CONFIG_BSP_USING_I2C),y)
  SRC_DIR += i2c
endif

ifeq ($(CONFIG_BSP_USING_SDIO),y)
  SRC_DIR += sdio
endif

ifeq ($(CONFIG_BSP_USING_USB),y)
  SRC_DIR += usb
endif

ifeq ($(CONFIG_BSP_USING_RTC),y)
  SRC_DIR += rtc
endif

ifeq ($(CONFIG_BSP_USING_WDT),y)
  SRC_DIR += watchdog
endif

ifeq ($(CONFIG_BSP_USING_TIMER),y)
  SRC_DIR += timer
endif

ifeq ($(CONFIG_BSP_USING_CAN),y)
  SRC_DIR += can
endif

include $(KERNEL_ROOT)/compiler.mk
