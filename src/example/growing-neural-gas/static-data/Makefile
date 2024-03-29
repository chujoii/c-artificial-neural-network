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

all : unit-testing example-gng gng-to-dot_unit-testing

# create object file
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# generate revision number
revision :
	git describe --always master > revision.txt

# unit-testing for "growing neural gas"
UNIT-TESTING_SRC=../../../growing-neural-gas.c ../../../vector.c unit-testing.o
unit-testing : unit-testing.o
	$(CC) $(CFLAGS) $(UNIT-TESTING_SRC) -lm -o $@

# example for "growing neural gas"
EXAMPLE_SRC=../../../growing-neural-gas.c ../../../vector.c example.o
example-gng : example.o
	$(CC) $(CFLAGS) $(EXAMPLE_SRC) -lm -o $@

# gng-to-dot_unit-testing for visualization "growing neural gas" with graphviz
GNG-TO-DOT_UNIT-TESTING_SRC=../../../growing-neural-gas.c ../../../vector.c ../../../gng-to-dot.c gng-to-dot_unit-testing.o
gng-to-dot_unit-testing : gng-to-dot_unit-testing.o
	$(CC) $(CFLAGS) $(GNG-TO-DOT_UNIT-TESTING_SRC) -lm -o $@

# installation process
#install :

# uninstall distribution
#uninstall :

# clean directory
clean :
	rm *.o unit-testing example-gng gng-to-dot_unit-testing

