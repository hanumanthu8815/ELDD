obj-m := mod_1/fun.o mod_2/avg.o
KDIR = /lib/modules/$(shell uname -r)/build/
PWD = $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
	depmod -a

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -rf /lib/modules/$(shell uname -r)/extra/*
	depmod -a 