obj-m:=seq.o
KD=/lib/modules/$(shell uname -r)/build/
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KD) M=$(PWD) modules
	

clean:
	$(MAKE) -C $(KD) M=$(PWD) clean
	

install:
	$(MAKE) -C $(KD) M=$(PWD) modules_install