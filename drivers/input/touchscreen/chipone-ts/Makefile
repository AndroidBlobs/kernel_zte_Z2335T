# Makefile for the chipone touchscreen drivers.

# Each configuration option enables a list of files.
ccflags-y += -I$(srctree)/drivers/input/touchscreen/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/chipone-ts/firmware_config/$(CONFIG_TS_FIRMWARE)/

obj-$(CONFIG_TOUCHSCREEN_CHIPONE) += cts_i2c_driver.o
obj-$(CONFIG_TOUCHSCREEN_CHIPONE) += cts_core.o
obj-$(CONFIG_TOUCHSCREEN_CHIPONE) += cts_platform.o
obj-$(CONFIG_TOUCHSCREEN_CHIPONE) += cts_firmware.o
obj-$(CONFIG_TOUCHSCREEN_CHIPONE) += icnt8xxx_flash.o
obj-$(CONFIG_TOUCHSCREEN_CHIPONE) += cts_tool.o
obj-$(CONFIG_TOUCHSCREEN_CHIPONE) += cts_sysfs.o
obj-$(CONFIG_TOUCHSCREEN_CHIPONE) += cts_test.o
obj-$(CONFIG_TOUCHSCREEN_CHIPONE) += cts_common_interface.o
