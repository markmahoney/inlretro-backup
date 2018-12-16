#compiler flags: -Wall Warnings all,  -g gdb debug markings, 
CFLAGS=-Wall -O
LDFLAGS= -llua -lusb-1.0 -Lsource/lua
INCLUDE= -I ./include -I ../shared
LDFLAGS_WINDOWS= -L ./winlib $(LDFLAGS)
LDFLAGS_UNIX= -lm $(LDFLAGS)
CC= gcc

SOURCES=$(wildcard source/*.c)
OBJECTS=$(SOURCES:.c=.o)

# Use GCC preprocessor to determine dependencies for all source files.
DEPS=$(OBJECTS:.o=.d)
%.d: %.c
	@$(CC) $(CFLAGS) $(INCLUDE) $< -MM -MT $(@:.d=.o) >$@
# Include generated .d Makefile fragments to manage object dependencies.
-include $(DEPS)

# Provide recipes for building all objects, dependencies managed via earlier inclusion.
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

# TODO: Pass through platform when building liblua.
liblua:
	$(MAKE) -C source/lua a

TARGET=inlretro

#default to windows build
all: liblua $(DEPS) $(OBJECTS)
	$(CC) $(INCLUDE) $(OBJECTS) -o $(TARGET) $(CFLAGS) $(LDFLAGS_WINDOWS)

#unix build doesn't need winlib directory as libusb should be installed on OS.
#sudo apt-get install libusb-1.0-0-dev
unix: liblua $(DEPS) $(OBJECTS)
	$(CC) $(INCLUDE) $(OBJECTS) -o $(TARGET) $(CFLAGS) $(LDFLAGS_UNIX)

# "make debug" will build program with debug print messages
# -DDEBUG show debug logs
# -g build with gdb debug tags
debug: CFLAGS += -DDEBUG -g
debug: all

#unix debug build
unixdebug: CFLAGS += -DDEBUG -g
unixdebug: unix

#clean on unix and windows(.exe)
.PHONY: clean
clean:
	$(MAKE) -C source/lua clean
	rm -f $(TARGET) $(TARGET).exe $(OBJECTS) $(DEPS) $(LUAOBJ) source/lua/liblua.a
