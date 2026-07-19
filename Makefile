GCC=g++
FLAGS=-std=c++23 -fmodules-ts
#FLAGS=-std=c++23 -fmodules-ts 

all:
	$(GCC)   $(FLAGS) -c lib/lattice_storage/lattice_storage.cppm
	$(GCC)   $(FLAGS) -c lib/rel_operator/rel_operator.cppm			
	$(GCC) bin/main.C -o bin/main $(FLAGS)
