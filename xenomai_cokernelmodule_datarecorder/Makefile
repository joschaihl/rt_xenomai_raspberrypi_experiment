ARCH=arm
MODULES = datarecorder

KSRC ?= /lib/modules/$(shell uname -r)/build
#KSRC ?= /lib/modules/3.2.21+/build
OBJS     := ${patsubst %, %.o, $(MODULES)}
CLEANMOD := ${patsubst %, .%*, $(MODULES)}
PWD      := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
USPACE_CFLAGS	:=	`xeno-config --skin native --cflags`
USPACE_LDFLAGS	:=	`xeno-config --skin native --ldflags`

obj-m        := $(OBJS)
EXTRA_CFLAGS := -I$(KSRC)/include/xenomai -I$(KSRC)/include/xenomai/posix $(ADD_CFLAGS)

all::
	$(MAKE) -C $(KSRC) ARCH=$(ARCH) SUBDIRS=$(PWD) modules
	gcc $(USPACE_CFLAGS) $(USPACE_LDFLAGS) read_shm.c -o read_shm
	$(RM) *.o

clean::
	$(RM) $(CLEANMOD) *.o *.ko *.mod.c Module*.symvers Module.markers modules.order read_shm
	$(RM) -R .tmp*
	