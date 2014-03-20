PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64
all:
		g++ p3.cpp -o market -std=c++11 -O3
clean:
		rm -f *.o
		rm -f market
