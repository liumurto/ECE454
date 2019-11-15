CC = g++
CFLAGS = -O3 -lpthread -fgnu-tm

all: randtrack_tm

randtrack_tm: list.h hash.h defs.h randtrack_tm_lock.cc
	$(CC) $(CFLAGS) randtrack_tm_lock.cc -o randtrack_tm


clean:
	rm -f *.o randtrack randtrack_global_lock randtrack_tm randtrack_list_lock
