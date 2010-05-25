# @source      Makefile
# @description makefile for the application

# call with "make DEBUG=1" to build with debug

ifdef DEBUG
FLAG_DEBUG=-g
endif

# sources directories
srcdir=../..
libdir=../../../../library
incdir=$(libdir)/source
vpath %.c $(srcdir)
vpath %.h $(srcdir) : $(incdir) : $(incdir)/util

main=Client_1
bin=$(main)

includes= Client.h

objects= $(main).o

CFLAGS += -Wall -Wextra -Werror -fpack-struct -Wno-main
# -Wall: emits many many warnings
# -Wextra: emits even more warnings
# -Werror: treats warnings as errors
# -Wno-main: accepts "void main"

# Linux runtime thread support
LDFLAGS+=-lpthread

# rule for compilation of c files
%.o: %.c $(includes)
	@echo
	@echo "* compiling $<"
	$(CC) -I$(incdir) -I$(incdir)/util $(FLAG_DEBUG) -c $(CFLAGS) $(CPPFLAGS) $<
	@echo

# creation of executable binary
$(bin): $(objects)
	@echo
	@echo "* linking $@"
	$(CC) -o $@ $^ $(LDFLAGS) -lTcpClient2 -L $(libdir)/build/linux
	@echo

# removing files to force rebuilding
.PHONY: clean
clean:
	@echo
	@echo "* cleaning files"
	@-rm $(bin) $(objects) > /dev/null 2>&1
	@echo

# running the application
.PHONY: run
run:
	@echo
	@echo "* running application"
	@LD_LIBRARY_PATH=$(libdir)/build/linux:$LD_LIBRARY_PATH ./$(bin)
	@echo "* end of application"

# the end
