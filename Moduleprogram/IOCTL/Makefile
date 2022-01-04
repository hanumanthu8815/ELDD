 obj-m := IOCTL.o
 KDIR = /lib/modules/$(shell uname -r)/build/
 PWD = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -o IOCTL_app IOCTL_app.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm IOCTL_app

install:
	$(MKAE) -C $(KDIR) M=$(PWD) modules_install
