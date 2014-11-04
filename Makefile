obj-m := bnull.o

bnull-y := \
bnull_main.o \
bnull_devops.o \
bnull_getgeo.o \

EXTRA_CFLAGS := -O2 -g -Wall -Werror #-Winline
all: mod

mod :
	+@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean :
	+@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
