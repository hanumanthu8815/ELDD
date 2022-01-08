obj-m := sema.o
KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -o r1 r1.c
	gcc -o r2 r2.c
	gcc -o r3 r3.c
	gcc -o w1 w1.c
	gcc -o w2 w2.c
	gcc -o w3 w2.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm r1 r2 r3 w1 w2 w3

install:
	$(MKAE) -C $(KDIR) M=$(PWD) modules_install
