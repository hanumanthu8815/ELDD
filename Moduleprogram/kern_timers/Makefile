obj-m := timers.o
KDIR = /lib/modules/$(shell uname -r)/build/
PWD = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	

install:
	$(MKAE) -C $(KDIR) M=$(PWD) modules_install
