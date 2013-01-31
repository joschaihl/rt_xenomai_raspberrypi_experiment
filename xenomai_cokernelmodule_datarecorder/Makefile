ARCH=arm
MODULES = control_pipe rec_state datarecorder
KSRC ?= /lib/modules/$(shell uname -r)/build
#KSRC ?= /lib/modules/3.2.21+/build
OBJS     := ${patsubst %, %.o, $(MODULES)}
#OBJS = print.o ringbuffer_model.o control_pipe.o rec_state.o
CLEANMOD := ${patsubst %, .%*, $(MODULES)}
PWD      := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
USPACE_CFLAGS	:=	`xeno-config --skin native --cflags`
USPACE_LDFLAGS	:=	`xeno-config --skin native --ldflags`
EXTRA_CFLAGS := -I$(KSRC)/include/xenomai -I$(KSRC)/include/xenomai/posix $(ADD_CFLAGS)

obj-m        := $(OBJS)
#obj-m += submodules.o
#datarecorder-objs := control_pipe.o rec_state.o

#obj-m += ringbuffer_model.o
#obj-m += control_pipe.o
#obj-m += rec_state.o

#obj-m += $(TARGET).o
#obj


#default:
#	$(MAKE) -C $(KSRC) ARCH=$(ARCH) SUBDIRS=$(PWD) modules
#$(TARGET).o: $(OBJS)

all::
	$(MAKE) -C $(KSRC) ARCH=$(ARCH) SUBDIRS=$(PWD) modules
	gcc $(USPACE_CFLAGS) $(USPACE_LDFLAGS) read_shm.c -o read_shm
	$(RM) *.o

clean::
	$(RM) $(CLEANMOD) *.o *.ko *.mod.c Module*.symvers Module.markers modules.order read_shm
	$(RM) -R .tmp*
	