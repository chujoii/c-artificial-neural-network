CC      = gcc -std=gnu99
# march:
# gcc -c -Q -march=native --help=target
# so for beagle correct: -march=armv7-a
# or auto:               -march=native
#
# pipe: has no effect on the generated code,
#       but it makes the compilation process faster
CFLAGS = -Wall -Wextra -pedantic -O2 -pipe -D_GNU_SOURCE

UNAME := $(shell uname -a)
ifneq ($(filter x86%,$(UNAME)),)
	CFLAGS += -march=native
endif
ifneq ($(filter armv7l,$(UNAME)),)
	CFLAGS += -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=hard
endif

# Berkeley DB
DB	= -ldb
#DB	= -l:libdb-4.4.so

all : gng-to-dot.o  growing-neural-gas.o vector.o

# create object file
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# installation process
#install :

# uninstall distribution
#uninstall :

# clean directory
clean :
	rm *.o

