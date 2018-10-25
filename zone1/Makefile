# Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved

TARGET = zone1.elf

CFLAGS += -O0 -fno-builtin-printf -msmall-data-limit=8 -std=gnu11
LDFLAGS += --specs=nosys.specs -Xlinker --gc-sections

override BOARD := $(subst e51,e31,$(BOARD))
override BSP_BASE := $(CURDIR)/bsp

C_SRCS += main.c

INCLUDES += -I../multizone/libhexfive
LDFLAGS += -L../multizone/libhexfive
LDFLAGS += $(if $(findstring rv64, $(RISCV_ARCH)), -lhexfive64, -lhexfive32)

include $(BSP_BASE)/env/common.mk


