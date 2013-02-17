include ../Makefile.conf
DEBUG_DIR	= debug
RELEASE_DIR = release
U_TOOLS = read_shm_json read_shm_html read_shm_raw read_shm_png
TEST_RUNNER = runner
MODULES = ArgumentParser RingBufferConsumer RingBufferPageIndex RingBufferLastDataIndex
TEST_MODULES = $(MODULES)
U_MODULES = $(TEST_MODULES) $(TEST_RUNNER)
OBJS     	 := ${patsubst %, %.o, $(U_MODULES)}
TEST_OBJS	 := ${patsubst %, %.o, $(U_MODULES)}
TEST_HEADERS := ${patsubst %, %_test.h, $(TEST_MODULES)}
#DEFAULT_FLAGS = $(DEBUG_FLAG) $(GCOV_FLAG)

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

read_shm_png: ArgumentParser.o SensorDataPrinterTool.o RingBufferConsumer.o RingBufferPageIndex.o RingBufferLastDataIndex.o read_shm_png.o 
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_LDFLAGS) $(OPENCV_LDFLAGS) ArgumentParser.o SensorDataPrinterTool.o RingBufferConsumer.o RingBufferPageIndex.o RingBufferLastDataIndex.o read_shm_png.o $(LINKER-FLAGS) $@
	
read_shm_png.o: read_shm_png.cpp SensorDataPrinterTool.h
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) $(OPENCV_CFLAGS) -c read_shm_png.cpp -o read_shm_png.o

read_shm_raw: ArgumentParser.o SensorDataPrinterTool.o RingBufferConsumer.o RingBufferPageIndex.o RingBufferLastDataIndex.o read_shm_raw.o
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_LDFLAGS) ArgumentParser.o SensorDataPrinterTool.o RingBufferConsumer.o RingBufferPageIndex.o RingBufferLastDataIndex.o read_shm_raw.o $(LINKER-FLAGS) $@

read_shm_raw.o: read_shm_raw.cpp SensorDataPrinterTool.h
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c read_shm_raw.cpp -o read_shm_raw.o
	
read_shm_html: ArgumentParser.o SensorDataPrinterTool.o RingBufferConsumer.o RingBufferPageIndex.o RingBufferLastDataIndex.o read_shm_html.o
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_LDFLAGS) ArgumentParser.o SensorDataPrinterTool.o RingBufferConsumer.o RingBufferPageIndex.o RingBufferLastDataIndex.o read_shm_html.o $(LINKER-FLAGS) $@

read_shm_html.o: read_shm_html.cpp SensorDataPrinterTool.h
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c read_shm_html.cpp -o read_shm_html.o
	
read_shm_json: ArgumentParser.o SensorDataPrinterTool.o RingBufferConsumer.o RingBufferPageIndex.o RingBufferLastDataIndex.o read_shm_json.o
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_LDFLAGS) SensorDataPrinterTool.o ArgumentParser.o RingBufferConsumer.o RingBufferPageIndex.o RingBufferLastDataIndex.o read_shm_json.o $(LINKER-FLAGS) $@
	
read_shm_json.o: read_shm_json.cpp SensorDataPrinterTool.h
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c read_shm_json.cpp -o read_shm_json.o

SensorDataPrinterTool.o: SensorDataPrinterTool.cpp SensorDataPrinterTool.h IIncrementableIndex.h RingBufferConsumer.h RingBufferPageIndex.h RingBufferLastDataIndex.h
		$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c SensorDataPrinterTool.cpp -o SensorDataPrinterTool.o
	
ArgumentParser.o: ArgumentParser.cpp ArgumentParser.h
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c ArgumentParser.cpp -o ArgumentParser.o
	
RingBufferConsumer.o: RingBufferConsumer.cpp RingBufferConsumer.h DataRecorderExceptions.h
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c RingBufferConsumer.cpp -o RingBufferConsumer.o

RingBufferPageIndex.o: IIncrementableIndex.h RingBufferConsumer.h RingBufferPageIndex.h RingBufferPageIndex.cpp
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c RingBufferPageIndex.cpp -o RingBufferPageIndex.o

RingBufferLastDataIndex.o: IIncrementableIndex.h RingBufferConsumer.h RingBufferLastDataIndex.h RingBufferLastDataIndex.cpp
	$(C++) $(C++-STD-FLAG) $(WARNING_FLAGS) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c RingBufferLastDataIndex.cpp -o RingBufferLastDataIndex.o

runner: $(TEST_OBJS) $(TEST_HEADERS)
	$(C++) $(C++-STD-FLAG) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_LDFLAGS) $(OBJS) $(LINKER-FLAGS) runner

runner.o: runner.cpp $(TEST_HEADERS)
	$(C++) $(C++-STD-FLAG) $(C++-STD-FLAG) $(GCOV_FLAGS) $(DEBUG_FLAG) $(USPACE_CFLAGS) -c runner.cpp -o runner.o

runner.cpp: $(TEST_HEADERS)
	$(CXXTESTGEN) $(CXXTESTGEN_GEN_FLAGS) runner.cpp $(TEST_HEADERS)
	
install: clean
	$(MAKE) all GCOV_FLAGS=$(OFLAGS) DEBUG_FLAG=-Wall
	sudo $(INSTALL) $(U_TOOLS) /sbin
        
doc:
	$(DOXYGEN)
	make -C latex

test: runner
	- sudo $(MEMLEAK_CHECK) ./runner
	sudo su -c ./runner

coverage: gcovclean test
	#lcov --base-directory . --directory . --zerocounters -q
	#lcov --zerocounters --directory .
	lcov --base-directory . --directory . -c -o coverage.info
	lcov --remove coverage.info "/usr*" -o coverage.info # remove output for external libraries
	rm -rf test_coverage
	genhtml -o test_coverage -t "Xenomai User Space Tools Test Coverage" --num-spaces 4 coverage.info
	
	
	
# remove object files and core (if any)
clean: gcovclean
	$(RM) $(TARGET)
	$(RM) runner.cpp
	$(RM) *.o core ._*
	$(RM) -fr latex html

	
gcovclean:
	$(RM) *.gcov *.gcda *.gcno
	$(RM) -rf test_coverage test_coverage.info coverage.info

# remove object files, core dump, and executable (if any)
distclean:
	$(MAKE) clean

.PHONY: all

