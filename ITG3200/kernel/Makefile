#
# Makefile for miscellaneous I2C chip drivers.
# 

CROSSCOMPILE=/opt/codesourcery/arm-2009q1/bin/arm-none-linux-gnueabi-
CC=$(CROSSCOMPILE)gcc
ARCH=arm

obj-m	+= itg3200.o

KDIR=/opt/virt2real/kernel/linux-2.6.32.17-psp03.01.01.39

all:
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSSCOMPILE) M=$(PWD) -I $(KDIR)/include modules

clean:
	make -C $(KDIR) M=$(PWD) clean
