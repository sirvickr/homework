#gcc -o test.bin -I/opt/mpich/include ./main.c

INCPATH=-I/usr/lib64/mpi/gcc/openmpi/include
LIBPATH=-L/usr/lib64/mpi/gcc/openmpi/lib64

task: main.cpp
	mpic++ -std=c++11 -o task $(INCPATH) $(LIBPATH) ./main.cpp -lmpi -lstdc++ -lmpi
