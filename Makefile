# Define compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra

# Kernel directory for module build
KDIR := /lib/modules/$(shell uname -r)/build

# Targets for make
all: clean modules app

# Build the kernel module
obj-m += cpu_profiler.o
modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Build the user application
app:
	$(CC) $(CFLAGS) -o userapp userapp.c

# Clean up the build artifacts
clean:
	rm -f userapp *.ko *.o *.mod *.mod.c *.symvers *.order .*.cmd *.mod.o

# Help target to display callable targets and their description
help:
	@echo "Usage:"
	@echo "  make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all     Builds all targets"
	@echo "  modules Build the kernel module"
	@echo "  app     Build the user space application"
	@echo "  clean   Remove all generated files"
