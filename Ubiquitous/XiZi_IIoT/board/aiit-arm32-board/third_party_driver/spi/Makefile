SRC_FILES := hardware_spi.c connect_spi.c 

ifeq ($(CONFIG_RESOURCES_SPI_SFUD),y)
	SRC_FILES += connect_flash_spi.c
endif

ifeq ($(CONFIG_RESOURCES_SPI_LORA),y)
	SRC_DIR += third_party_spi_lora
	SRC_FILES += connect_lora_spi.c
endif

include $(KERNEL_ROOT)/compiler.mk
