# Sun Jun 28 20:41:10 WIB 2020
# Thu Jun 11 17:35:49 WIB 2020

CC      = gcc
SOURCES = $(wildcard p*.c)
EXECS   = $(SOURCES:%.c=%)
SHAREMEM= SharedMemoryFile.bin
CFLAGS  = -std=gnu11 -pthread

all: $(EXECS) 

$(EXECS): share.c share.h

clean:
	rm -f $(EXECS) $(SHAREMEM)

.PHONY: all clean
