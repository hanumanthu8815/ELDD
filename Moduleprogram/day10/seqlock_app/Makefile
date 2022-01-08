obj-m:=seq.o
KD=/lib/modules/$(shell uname -r)/build/
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KD) M=$(PWD) modules
	gcc -o w1 w1.c
	gcc -o r1 r1.c
	gcc -o r2 r2.c

clean:
	$(MAKE) -C $(KD) M=$(PWD) clean
	rm w1 r1 r2
	

install:
	$(MAKE) -C $(KD) M=$(PWD) modules_install

