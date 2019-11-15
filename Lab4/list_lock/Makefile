CC = g++
CFLAGS = -O3 -lpthread -fgnu-tm

all: randtrack_list_lock

randtrack_list_lock: list.h hash.h defs.h randtrack_list_lock.cc
	$(CC) $(CFLAGS) -lpthread randtrack_list_lock.cc -o randtrack_list_lock


clean:
	rm -f *.o randtrack randtrack_global_lock randtrack_tm randtrack_list_lock
