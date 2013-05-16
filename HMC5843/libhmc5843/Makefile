#
# Makefile for hmc5843 access library via I2C and hmc5843 module
#
#
GCC=arm-angstrom-linux-gnueabi-gcc
#GCC=gcc
INCLUDE_DIR=/usr/local/openmoko/arm/arm-angstrom-linux-gnueabi
#INCLUDE_DIR=/


all:
	valac -C -H hmc5843.h --library hmc5843 hmc5843.vala --basedir ./
	sed s/\<hmc5843.h\>/\"hmc5843.h\"/g hmc5843.c > hmc5843updated.c
	${GCC} --shared -fPIC -I${INCLUDE_DIR}/usr/include/glib-2.0 -I${INCLUDE_DIR}/usr/lib/glib-2.0/include -I. -o libhmc5843.so $(pkg-config --cflags --libs gobject-2.0) hmc5843updated.c
	#valac -X -I. -X -L. -X -lhmc5843 -o test test.vala hmc5843.vapi --basedir ./

clean:
	rm *.c *.h *.vapi *.o *.so test
