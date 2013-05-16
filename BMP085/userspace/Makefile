CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar

ALL: test libbmp085.so libbmp085.a

libbmp085.so: bmp085.o
	$(CC) -shared -Wl,-soname,libbmp085.so -o libbmp085.so bmp085.o

libbmp085.a: bmp085.o
	$(AR) rcs libbmp085.a bmp085.o

bmp085.o: bmp085.c
	$(CC) -c -fPIC -o bmp085.o bmp085.c

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f test_bmp085
