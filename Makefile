GCC=g++
FLAGS=-std=c++23 -fmodules-ts
#FLAGS=-std=c++23 -fmodules-ts 

all:
	$(GCC)   $(FLAGS) -c lib/lattice_storage/lattice_storage.cppm
	$(GCC)   $(FLAGS) -c lib/rel_operator/rel_operator.C			
	$(GCC) bin/main.C  lib/rel_operator/rel_operator.C lib/lattice_storage/lattice_storage.C -o bin/main $(FLAGS)
