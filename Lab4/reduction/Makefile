CC = g++
CFLAGS = -O3 -lpthread -fgnu-tm

all: randtrack_reduction

randtrack_reduction: list.h hash.h defs.h randtrack_reduction.cc
	$(CC) $(CFLAGS) -lpthread randtrack_reduction.cc -o randtrack_reduction


clean:
	rm -f *.o randtrack randtrack_global_lock randtrack_tm randtrack_list_lock
