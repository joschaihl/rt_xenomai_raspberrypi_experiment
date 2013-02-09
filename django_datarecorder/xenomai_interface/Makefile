include ../../Makefile.conf
XENOMAI_USERSPACE_TOOLS_DIR = ../../xenomai_userspace_tools
DATARECORDER_LIB_DIR = ../../datarecorder_lib
TARGETS = _ringbufferconsumer.so
all: $(TARGETS)

install: all

# RingBufferConsumer
_ringbufferconsumer.so: RingBufferConsumer.o
	$(C++) $(XENO_LDFLAGS) -shared   \
	$< $(patsubst %.o, %_wrap.o, $<)    -o $@
	
	
RingBufferConsumer.o: RingBufferConsumer_wrap.cxx
	$(C++) -I$(DATARECORDER_LIB_DIR) -I$(XENOMAI_USERSPACE_TOOLS_DIR) \
	 $(PYTHON_INCLUDES) $(XENO_CFLAGS) \
	 $(PACKED_CFLAGS) -c -fpic $< \
	 $(XENOMAI_USERSPACE_TOOLS_DIR)/$(patsubst %.o,%.cpp,$@)
	
RingBufferConsumer_wrap.cxx: $(XENOMAI_USERSPACE_TOOLS_DIR)/RingBufferConsumer.i
	$(SWIG_CPP_TO_PYTHON) -outcurrentdir $<
	
clean:
	$(RM) $(TARGETS)
	$(RM) _*.so *.o *.cxx $(patsubst _%.so,%.*,$(TARGETS))

.PHONY: all