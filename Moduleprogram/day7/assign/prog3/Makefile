 obj-m := p3.o
 KDIR = /lib/modules/$(shell uname -r)/build/
 PWD = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -o app app.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm app

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
