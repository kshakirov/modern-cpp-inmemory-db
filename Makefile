GCC=g++
FLAGS=-std=c++23 
#FLAGS=-std=c++23 -fmodules-ts 

all:


	$(GCC) bin/main.C  lib/rel_operator/rel_operator.C lib/lattice_storage/lattice_storage.C -o bin/main $(FLAGS)

coroutine:
	$(GCC) test/test_coroutince.   -o test/coroutince $(FLAGS)
