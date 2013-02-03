# from http://lackof.org/taggart/hacking/make-example/

include Makefile.conf

DIRS = xenomai_cokernelmodule_datarecorder xenomai_userspace_tools
BUILDDIRS = $(DIRS:%=build-%)
INSTALLDIRS = $(DIRS:%=install-%)
CLEANDIRS = $(DIRS:%=clean-%)
TESTDIRS = $(DIRS:%=test-%)

all: $(BUILDDIRS)
	@echo "==== Make done ===="
$(DIRS): $(BUILDDIRS)
$(BUILDDIRS):
	@echo "==== Make $(@:build-%=%) ===="
	$(MAKE) -C $(@:build-%=%)

## the utils need the libraries in dev built first
#build-utils: build-dev

install: $(INSTALLDIRS)
	@echo "==== Installing done ===="
	
$(INSTALLDIRS):
	@echo "==== Installing $(@:install-%=%) ===="
	$(MAKE) -C $(@:install-%=%) install

#test: $(TESTDIRS) all
#$(TESTDIRS): 
#	@echo "Test"#
#	$(MAKE) -C $(@:test-%=%) test

commit: clean
	@echo "===Adding new files==="
	$(GIT) add -A
	@echo "===Commiting==="
	$(GIT) commit
	$(GIT) push


clean: $(CLEANDIRS)
	@echo "==== Cleaning ===="
	$(RM) ._*
	
$(CLEANDIRS): 
	@echo "==== Cleaning $(@:clean-%=%) ===="
	$(MAKE) -C $(@:clean-%=%) clean


.PHONY: subdirs $(DIRS)
.PHONY: subdirs $(BUILDDIRS)
.PHONY: subdirs $(INSTALLDIRS)
.PHONY: subdirs $(TESTDIRS)
.PHONY: subdirs $(CLEANDIRS)
.PHONY: all install clean test