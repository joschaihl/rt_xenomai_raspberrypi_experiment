include ../Makefile.conf
U_TOOLS = read_shm_json
TEST_RUNNER = runner
MODULES = ArgumentParser RingBufferConsumer RingBufferIndex 
TEST_MODULES = $(MODULES)
U_MODULES = $(TEST_MODULES) $(TEST_RUNNER)
#read_shm read_shm_raw read_shm_json read_shm_html shm_properties
OBJS     	 := ${patsubst %, %.o, $(U_MODULES)}
TEST_OBJS	 := ${patsubst %, %.o, $(U_MODULES)}
TEST_HEADERS := ${patsubst %, %_test.h, $(TEST_MODULES)}

TARGET       = runner $(U_TOOLS)
#read_shm_raw read_shm_json read_shm_html shm_properties        # name of executable

all: $(TARGET)

#read_shm_raw: read_shm.o read_shm_raw.o 
#	$(C++) read_shm.o read_shm_raw.o  $(USPACE_LDFLAGS) $(LINKER-FLAGS)  $@ 
#	
#read_shm_json: read_shm.o read_shm_json.o
#	$(C++) read_shm.o read_shm_json.o $(USPACE_LDFLAGS) $(LINKER-FLAGS)  $@ 
#	
#read_shm_html: read_shm.o read_shm_html.o
#	$(C++) read_shm.o read_shm_html.o $(USPACE_LDFLAGS) $(LINKER-FLAGS)  $@ 
#	
#shm_properties: shm_properties.o
#	$(C++) $< $(USPACE_LDFLAGS) $(LINKER-FLAGS)  $@ 

#runner: runner.o ArgumentParser.o
#	$(C++) $< $(USPACE_LDFLAGS) $(LINKER-FLAGS)  $@ 

#%.o: %.cpp
#	$(C++) $(USPACE_CFLAGS) -c $< -o $@

read_shm_json: ArgumentParser.o RingBufferConsumer.o read_shm_json.o
	$(C++) $(DEBUG_FLAG) $(USPACE_LDFLAGS) ArgumentParser.o RingBufferConsumer.o read_shm_json.o $(LINKER-FLAGS) read_shm_json
	
read_shm_json.o: read_shm_json.cpp ArgumentParser.h RingBufferConsumer.h
	$(C++) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c read_shm_json.cpp -o read_shm_json.o
	
ArgumentParser.o: ArgumentParser.cpp ArgumentParser.h
	$(C++) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c ArgumentParser.cpp -o ArgumentParser.o
	
RingBufferConsumer.o: RingBufferConsumer.cpp RingBufferConsumer.h DataRecorderExceptions.h
	$(C++) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c RingBufferConsumer.cpp -o RingBufferConsumer.o

RingBufferIndex.o: RingBufferIndex.h RingBufferIndex.cpp
	$(C++) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c RingBufferIndex.cpp -o RingBufferIndex.o

runner: $(TEST_OBJS)
	$(C++) $(DEBUG_FLAG) $(USPACE_LDFLAGS) $(OBJS) $(LINKER-FLAGS) runner

runner.o: runner.cpp
	$(C++) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c runner.cpp -o runner.o

runner.cpp: $(TEST_HEADERS)
	$(CXXTESTGEN) $(CXXTESTGEN_GEN_FLAGS) runner.cpp $(TEST_HEADERS)
	
install:
	$(INSTALL) $(U_TOOLS) /sbin

doc:
	$(DOXYGEN)
	make -C latex

test: runner
	- sudo $(MEMLEAK_CHECK) ./runner
	sudo ./runner
	
	
# remove object files and core (if any)
clean:
	$(RM) $(TARGET)
	$(RM) runner.cpp
	$(RM) *.o core ._*
	$(RM) -fr latex html

# remove object files, core dump, and executable (if any)
distclean:
	$(MAKE) clean

.PHONY: all

