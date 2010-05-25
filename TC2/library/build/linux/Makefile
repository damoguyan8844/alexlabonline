# @source      Makefile
# @description makefile for the application
# See licensing information in the file README.TXT.

# call with "make DEBUG=1" to build with debug

ifdef DEBUG
FLAG_DEBUG=-g
endif

# sources directories
srcdir=../../source
vpath %.c $(srcdir) : $(srcdir)/util
vpath %.h $(srcdir) : $(srcdir)/util

name=TcpClient2
majorVersion=1
minorVersion=0
microVersion=2
lib=lib$(name).so.$(majorVersion).$(minorVersion).$(microVersion)
link0=lib$(name).so.$(majorVersion)
link1=lib$(name).so
zip=$(name).$(majorVersion).$(minorVersion).$(microVersion)

includes=               \
   config.h             \
   Client.h             \
   GenericClient.h      \
   Log.h                \
   Message.h            \
   Mutex.h              \
   Socket.h             \
   Thread.h             \
   Time.h               \
   Timeout.h

objects=                \
   Client.o             \
   GenericClient.o      \
   Log.o                \
   Message.o            \
   Mutex.o              \
   Socket.o             \
   Thread.o             \
   Time.o               \
   Timeout.o

CFLAGS += -Wall -Wextra -Werror -DREENTRANT -fpack-struct
# -Wall: emits many many warnings
# -Wextra: emits even more warnings
# -Werror: treats warnings as errors  

# Linux runtime thread support
LDFLAGS+=-lpthread

# rule for compilation of c files
%.o: %.c $(includes)
	@echo
	@echo "* compiling $<"
	$(CC) $(FLAG_DEBUG) -c -fPIC $(CFLAGS) $(CPPFLAGS) $<
	@echo

# creation of library
$(lib): $(objects)
	@echo
	@echo "* linking $@"
	$(CC) -shared -Wl,-soname,lib$(name).so.$(majorVersion) $(LDFLAGS) -o $@ $^
	-rm $(link0) > /dev/null 2>&1
	-rm $(link1) > /dev/null 2>&1
	ln -s $(lib) $(link0)
	ln -s $(link0) $(link1)
	#mv $(lib) ~/lib/$(lib)
	#mv $(link0) ~/lib/$(link0)
	#mv $(link1) ~/lib/$(link1)
	@echo

# creation of documentation files
.PHONY: html
html:
	@-( cd ../../source ; rm -rf ../doc/html/* ; doxygen )
	@-( cd ../../doc/html ; sed -i -e \
     's/"classes.png"/"classes.png" width=65% /' main.html )
	@-( cd ../../doc/html ; sed -i -e \
     's/"sequence.png"/"sequence.png" width=50% /' main.html )

# creation of compressed archive for distribution
.PHONY: zip
zip:
	@-( cd .. ; rm *.zip ; zip -r $(zip) . -x \*~ -x tags -x \*.vim -x \*.swp)

# removing files to force rebuilding
.PHONY: clean
clean:
	@echo
	@echo "* cleaning files"
	@-rm $(lib) $(link0) $(link1) $(objects) > /dev/null 2>&1
	@echo

# the end
