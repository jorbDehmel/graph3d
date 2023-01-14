###########################

SUFFIX = -pedantic -Wall `jgame3d-flags`

###########################

graphdemo.out:	graphdemo.o graph3d.o
	clang++ -o graphdemo.out graph3d.o graphdemo.o $(SUFFIX)

simdemo.out:	simdemo.o sim3d.o graph3d.o
	clang++ -o simdemo.out simdemo.o graph3d.o sim3d.o $(SUFFIX)

###########################

graphdemo.o:	graphdemo.cpp
	clang++ -c -o graphdemo.o graphdemo.cpp $(SUFFIX)

simdemo.o:	simdemo.cpp
	clang++ -c -o simdemo.o simdemo.cpp $(SUFFIX)

###########################

graph3d.o:	graph3d.cpp graph3d.hpp
	clang++ -c graph3d.cpp -o graph3d.o $(SUFFIX)

sim3d.o:	sim3d.cpp sim3d.hpp
	clang++ -c sim3d.cpp -o sim3d.o $(SUFFIX)

###########################

pclean:
	rm *.o *.out

###########################