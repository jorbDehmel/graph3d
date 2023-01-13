###########################

SUFFIX = -pedantic -Wall `jgame3d-flags`

###########################

main.out:	main.o graph3d.o
	clang++ -o main.out graph3d.o main.o $(SUFFIX)

###########################

main.o:	main.cpp
	clang++ -c main.cpp -o main.o $(SUFFIX)

graph3d.o:	graph3d.cpp graph3d.hpp
	clang++ -c graph3d.cpp -o graph3d.o $(SUFFIX)

###########################

pclean:
	rm *.o *.out

###########################