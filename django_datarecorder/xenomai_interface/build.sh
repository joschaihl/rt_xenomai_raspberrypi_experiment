XENOMAI_USERSPACE_TOOLS_DIR=../../xenomai_userspace_tools
DATARECORDER_LIB_DIR=../../datarecorder_lib

#echo $DATARECORDER_LIB_DIR
swig -python -c++ -outcurrentdir $XENOMAI_USERSPACE_TOOLS_DIR/RingBufferConsumer.i
g++ -I$DATARECORDER_LIB_DIR -I$XENOMAI_USERSPACE_TOOLS_DIR \
	 -I/usr/include/python2.7 -I/usr/lib/python2.7/config `xeno-config --skin native --cflags` \
	 -Wpacked-bitfield-compat -c -fpic RingBufferConsumer_wrap.cxx \
	 $XENOMAI_USERSPACE_TOOLS_DIR/RingBufferConsumer.cpp
g++ `xeno-config --skin native --ldflags` -shared   \
	 RingBufferConsumer.o RingBufferConsumer_wrap.o    -o _ringbufferconsumer.so