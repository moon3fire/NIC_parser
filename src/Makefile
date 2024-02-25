CONFIG_MODULE_SIG=n

# Define the name of the module object file
obj-m += iplir.o

# Define the path to the kernel source directory
KDIR := /lib/modules/$(shell uname -r)/build

# Define the default target
all:
	# Invoke the kernel makefile with the current directory as a parameter
	make -C $(KDIR) M=$(PWD) modules

# Define the clean target
clean:
	# Invoke the kernel makefile with the current directory as a parameter
	make -C $(KDIR) M=$(PWD) clean
