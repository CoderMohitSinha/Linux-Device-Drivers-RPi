obj-m := helloworld_module.o


clean: RPI_clean


RPI_build: helloworld_module.c
	sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(PWD)/../linux M=$(PWD) modules

RPI_clean:
	sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(PWD)/../linux M=$(PWD) clean


PHONY: all clean RPI_build RPI_clean
