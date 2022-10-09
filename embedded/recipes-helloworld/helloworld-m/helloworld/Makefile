TARGET = helloworld
all := $(TARGET)
CFLAGS ?= -Wall -O
bindir = $(DESTDIR)/usr/bin/

OBJS = helloworld.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

install:
	install -d $(bindir)
	install -m 0755 $(TARGET) $(bindir)
                           
uninstall:
	${RM} $(bindir)/$(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS)

.PHONY: all clean install uninstall

