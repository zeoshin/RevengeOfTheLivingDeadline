#Enables C++11
PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

#Default Flags
FLAGS = -Wall -Wextra -pedantic -Wvla -std=c++11

# make release - will compile "all" with $(FLAGS) and the O3 flag
release: FLAGS += -O3
release: all

# make debug - will compile "all" with $(FLAGS) and the g flag
debug: FLAGS += -g
debug: all

# all: <your dependencies here>
# 	g++ $(FLAGS) 

# Make sure to compile object files with the -c flag

# ***EXAMPLE***
#
all: rotld.o
	 g++ $(FLAGS) rotld.o -o rotld
test: test.o
	 g++ $(FLAGS) test.o -o test
test.o: test.cpp
	 g++ $(FLAGS) -c test.cpp
rotld.o: rotld.cpp
	 g++ $(FLAGS) -c rotld.cpp
#
# ***END EXAMPLE****

# make clean - remove .o files.  You can modify it to remove whatever
clean:
	rm -f *.o rotld
