include ../../Makefile.conf
XENOMAI_USERSPACE_TOOLS_DIR = ../../xenomai_userspace_tools
DATARECORDER_LIB_DIR = ../../datarecorder_lib
TARGETS = _RingBufferConsumer.so _RingBufferPageIndex.so
all: $(TARGETS)

install: all

# RingBufferPageIndex
_RingBufferPageIndex.so: RingBufferPageIndex.o RingBufferConsumer.o
	$(C++) $(XENO_LDFLAGS) -shared   \
	$< $(patsubst %.o, %_wrap.o, $<) RingBufferConsumer.o RingBufferConsumer_wrap.o -o $@
	
	
RingBufferPageIndex.o: RingBufferPageIndex_wrap.cxx
	$(C++) -I$(DATARECORDER_LIB_DIR) -I$(XENOMAI_USERSPACE_TOOLS_DIR) \
	 $(PYTHON_INCLUDES) $(XENO_CFLAGS) \
	 $(PACKED_CFLAGS) -c -fpic $< \
	 $(XENOMAI_USERSPACE_TOOLS_DIR)/$(patsubst %.o,%.cpp,$@)
	
RingBufferPageIndex_wrap.cxx: $(XENOMAI_USERSPACE_TOOLS_DIR)/RingBufferPageIndex.i
	$(SWIG_CPP_TO_PYTHON) -outcurrentdir $<

# RingBufferConsumer
_RingBufferConsumer.so: RingBufferConsumer.o
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
	$(RM) RingBufferConsumer* RingBufferPageIndex*
	$(RM) _*.so *.o *.cxx $(patsubst _%.so,%.*,$(TARGETS))

.PHONY: all