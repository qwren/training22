PREFIX=/usr
LIBDIR=$(PREFIX)/lib
INCLUDEDIR=$(PREFIX)/include
#$(DESTDIR) is usally empty. rpmbuild needs it.
DESTDIR=

CC=gcc
CFLAGS=
LIBS=
INCLUDES=
AR=ar

all: libthreadpool.a 

libthreadpool.a: thread-pool.o
	$(AR) -r $@ $^

thread-pool.o: src/thread-pool.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(LIBS) $(INCLUDES)

install:
	@echo Copying library files to $(DESTDIR)/$(LIBDIR):
	@cp -rp libthreadpool.a	$(DESTDIR)/$(LIBDIR)/
	@echo Copying head files to $(DESTDIR)/$(DATADIR):
	@cp -rp src/thread-pool.h $(DESTDIR)/$(INCLUDEDIR)/

clean:
	rm -rf *.o \
	       *.a

uninstall:
	rm -rf	$(LIBDIR)/libthreadpool.a \
		$(INCLUDEDIR)/thread-pool.h

