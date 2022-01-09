obj-m:=Calc_Chardriver.o

KD=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KD) M=$(PWD) modules
	gcc -o add add.c
	gcc -o sub sub.c
	gcc -o mul mul.c
	gcc -o div div.c
clean:
	$(MAKE) -C $(KD) M=$(PWD) clean	
	rm add sub mul div

mknodes:
	mknod /dev/ADD c 255 0
	mknod /dev/SUB c 255 1
	mknod /dev/MUL c 255 2
	mknod /dev/DIV c 255 3