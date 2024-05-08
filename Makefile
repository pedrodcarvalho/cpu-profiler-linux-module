all: clean modules app

obj-m := cpu_profiler.o

modules: cpu_profiler.c cpu_profiler.h
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) CC=x86_64-linux-gnu-gcc-13 modules

app: userapp.c
	gcc -o userapp userapp.c

clean:
	rm -f userapp *.ko *.o *.mod *.symvers *.order *.mod.c *.mod.o .*.cmd
