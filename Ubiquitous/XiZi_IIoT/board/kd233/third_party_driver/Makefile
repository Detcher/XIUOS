SRC_FILES := sleep.c

ifeq ($(CONFIG_BSP_USING_DMA),y)
  SRC_DIR += dma
endif

ifeq ($(CONFIG_BSP_USING_GPIO),y)
  SRC_DIR += gpio
endif

ifeq ($(CONFIG_BSP_USING_I2C),y)
  SRC_DIR += i2c
endif

ifeq ($(CONFIG_BSP_USING_PLIC),y)
  SRC_DIR += plic
endif

ifeq ($(CONFIG_BSP_USING_RTC),y)
  SRC_DIR += rtc
endif

ifeq ($(CONFIG_BSP_USING_SPI),y)
  SRC_DIR += spi
endif

ifeq ($(CONFIG_BSP_USING_SYSCLOCK),y)
  SRC_DIR += sys_clock
endif

ifeq ($(CONFIG_BSP_USING_HWTIMER),y)
  SRC_DIR += timer
endif

ifeq ($(CONFIG_BSP_USING_UART),y)
  SRC_DIR += uart
endif

ifeq ($(CONFIG_BSP_USING_VIDEO),y)
  SRC_DIR += video
endif

ifeq ($(CONFIG_BSP_USING_WDT),y)
  SRC_DIR += watchdog
endif

include $(KERNEL_ROOT)/compiler.mk
