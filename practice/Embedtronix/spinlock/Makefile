 obj-m := chardriver.o
 KDIR = /lib/modules/$(shell uname -r)/build/
 PWD = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -o read read.c
	gcc -o write write.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm read
	rm write

install:
	$(MKAE) -C $(KDIR) M=$(PWD) modules_install
