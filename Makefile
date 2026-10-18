GCC=g++-16
FLAGS=-std=c++26
#FLAGS=-std=c++26 -fmodules-ts

all:


	$(GCC) bin/main.C lib/lattice_storage/lattice_storage.C -o bin/main $(FLAGS)

coroutine:
	$(GCC) test/test_coroutince.   -o test/coroutince $(FLAGS)

test_template:
	$(GCC) test/test_templates.C   -o test/test_templates $(FLAGS)

test_cartesian:
	$(GCC) test/test_cartesian_product_2.C   -o test/test_cartesian_product_2 $(FLAGS)	

test_select_indices:
	$(GCC) test/test_select_indices.C -o test/test_select_indices $(FLAGS)
