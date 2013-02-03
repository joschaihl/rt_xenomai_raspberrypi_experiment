# if DATARECORDER_DIR is not set it goes to the parent directory
ifndef MAKEFILE_CONF
	MAKEFILE_CONF=../Makefile.conf
endif

include $(MAKEFILE_CONF)

ARCH=arm
K_MODULES = control_pipe rec_state rec_ringbuffer datarecorder

KSRC ?= /lib/modules/$(shell uname -r)/build
#KSRC ?= /lib/modules/3.2.21+/build
KBIN ?= /lib/modules/$(shell uname -r)/kernel
OBJS     := ${patsubst %, %.o, $(K_MODULES)}
KOBJS		 := ${patsubst %, %.ko, $(K_MODULES)}
CLEANMOD := ${patsubst %, .%*, $(K_MODULES)}

EXTRA_CFLAGS := -I$(DATARECORDER_LIB) -I$(KSRC)/include/xenomai -I$(KSRC)/include/xenomai/posix $(ADD_CFLAGS) $(PACKED_CFLAGS)

obj-m        := $(OBJS)

all::
	$(MAKE) -C $(KSRC) MAKEFILE_CONF=$(PWD)/$(MAKEFILE_CONF) DATARECORDER_LIB=$(DATARECORDER_LIB) ARCH=$(ARCH) SUBDIRS=$(PWD) $(DATARECORDER_LIB) modules
	$(RM) *.o

install::
	mkdir -p $(KBIN)/datarecorder
	$(INSTALL) $(KOBJS) $(KBIN)/datarecorder
	depmod -a
	./restart.sh
	
clean::
	$(RM) $(CLEANMOD) *.o *.ko *.mod.c Module*.symvers Module.markers modules.order
	$(RM) .*.o .*.ko .*.cmd
	$(RM) -R .tmp*
	
