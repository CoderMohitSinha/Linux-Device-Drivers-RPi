obj-m := helloworld_module.o


clean: RPI_clean


RPI_build: helloworld_module.c
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

RPI_clean:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean


PHONY: all clean RPI_build RPI_clean
