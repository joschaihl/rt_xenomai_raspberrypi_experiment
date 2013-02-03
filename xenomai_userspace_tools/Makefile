include ../Makefile.conf
U_MODULES = read_shm
OBJS     := ${patsubst %, %.o, $(U_MODULES)}

TARGET       = read_shm        # name of executable

$(TARGET): $(OBJS)
	$(LINKER) $(USPACE_LDFLAGS) $(LINKER-FLAGS) $@ $(OBJS)

%.o: %.c
	$(CC) $(USPACE_CFLAGS) -c $< -o $@
	
install:
	cp $(U_MODULES) /sbin

# remove object files and core (if any)
clean:
	$(RM) *.o core *._*.*

# remove object files, core dump, and executable (if any)
distclean:
	$(MAKE) clean
	$(RM) $(TARGET)