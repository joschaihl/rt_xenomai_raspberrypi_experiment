ARCH=arm
K_MODULES = control_pipe rec_state rec_ringbuffer datarecorder
KSRC ?= /lib/modules/$(shell uname -r)/build
#KSRC ?= /lib/modules/3.2.21+/build
KBIN ?= /lib/modules/$(shell uname -r)/kernel
OBJS     := ${patsubst %, %.o, $(K_MODULES)}
KOBJS		 := ${patsubst %, %.ko, $(K_MODULES)}
CLEANMOD := ${patsubst %, .%*, $(K_MODULES)}
PWD      := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
USPACE_CFLAGS	:=	`xeno-config --skin native --cflags`
USPACE_LDFLAGS	:=	`xeno-config --skin native --ldflags`
EXTRA_CFLAGS := -I$(KSRC)/include/xenomai -I$(KSRC)/include/xenomai/posix $(ADD_CFLAGS)

obj-m        := $(OBJS)

all::
	$(MAKE) -C $(KSRC) ARCH=$(ARCH) SUBDIRS=$(PWD) modules
	$(RM) *.o
	
read_shm::
	gcc $(USPACE_CFLAGS) $(USPACE_LDFLAGS) read_shm.c -o read_shm

modules_install::
	mkdir -p $(KBIN)/datarecorder
	cp $(KOBJS) $(KBIN)/datarecorder
	depmod -a

clean::
	$(RM) $(CLEANMOD) *.o *.ko *.mod.c Module*.symvers Module.markers modules.order read_shm
	$(RM) .*.o .*.ko .*.cmd
	$(RM) -R .tmp*
	
