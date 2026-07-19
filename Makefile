GCC=g++
FLAGS=-std=c++23 -fmodules-ts 

all:
	$(GCC) bin/main.C -o bin/main $(FLAGS)
