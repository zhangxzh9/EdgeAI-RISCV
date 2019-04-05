TARGET = recognition
CFLAGS += -O2

BSP_BASE = ../../bsp

HEADERS := \
	data.h 

C_SRCS += recognition.c
C_SRCS += data.c
C_SRCS += $(BSP_BASE)/$(BOARD)/drivers/plic/plic_driver.c

include $(BSP_BASE)/$(BOARD)/env/common.mk
