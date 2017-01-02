CXXFLAGS=-I/usr/local/include -std=gnu++11 -Wall

all: microhttpd

microhttpd: microhttpd.o worker.o
	g++ $^ -o $@ -lmicrohttpd -lboost_system -lboost_thread -lpthread

worker.o: worker.cpp worker.hpp

clean:
	rm -rf *.o microhttpd
	
