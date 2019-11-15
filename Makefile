CC = g++
CFLAGS = -O3 -lpthread -fgnu-tm
basedir = $(CURDIR)

all: randtrack randtrack_global_lock randtrack_tm randtrack_list_lock randtrack_element_lock randtrack_reduction

randtrack:
	cd $(basedir)/original && make
	mv $(basedir)/original/randtrack $(basedir)

randtrack_global_lock:
	cd $(basedir)/global_lock && make
	mv $(basedir)/global_lock/randtrack_global_lock $(basedir)

randtrack_tm:
	cd $(basedir)/tm_lock && make
	mv $(basedir)/tm_lock/randtrack_tm $(basedir)

randtrack_list_lock:
	cd $(basedir)/list_lock && make
	mv $(basedir)/list_lock/randtrack_list_lock $(basedir)

randtrack_element_lock:
	cd $(basedir)/ele_lock && make
	mv $(basedir)/ele_lock/randtrack_element_lock $(basedir)

randtrack_reduction:
	cd $(basedir)/reduction && make
	mv $(basedir)/reduction/randtrack_reduction $(basedir)

clean:
	rm -f *.o randtrack randtrack_global_lock randtrack_tm randtrack_list_lock randtrack_element_lock randtrack_reduction