CC = g++
CFLAGS = -O3 -lpthread -fgnu-tm

all: randtrack_element_lock 

randtrack_element_lock: list.h hash.h defs.h randtrack_ele_lock.cc
	$(CC) $(CFLAGS) -lpthread randtrack_ele_lock.cc -o randtrack_element_lock


clean:
	rm -f *.o randtrack randtrack_global_lock randtrack_tm randtrack_list_lock
