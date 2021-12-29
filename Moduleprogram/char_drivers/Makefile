##ifneq($(KERNELRELEASE),)
        obj-m := CharDriver.o

##else
        KERNELDIR = /lib/modules/$(shell uname -r)/build
        PWD  := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o app app.c

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm app

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

#endif
