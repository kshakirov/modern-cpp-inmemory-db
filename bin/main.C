using namespace std;
#include <iostream>
#include "../lib/rel_operator/rel_operator.hpp"
#include "../lib/lattice_storage/lattice_storage.hpp"
#include <iostream>
#include <vector>
//#include <generator> // Требуе
#include <algorithm>

// 1. Тот самый структурный тип-массив


int main(int argc, char** argv){
  cout<< "Emulating our rel engine"<< endl;
  cout<<""<<endl;
  LatticeStorage storage;
  storage.id =1;
  Column<"id", int> id_column;
  auto r = id_column.insert(1);
  cout << "insereted " << r <<" items"<< endl;
  auto result = id_column.select_one(1);
  if (result)
    cout<< "Found an item " << *result << "  "<< endl;
  result = id_column.select_one(2);
  if (!result)
    cout<< "Can't Finnd an  item with value 2  "<< endl;
  result = id_column.select_one(2);
  
  id_column.print_name(); // Выведет: Column name: id
  Filter(&storage, 3);
  dump(&storage);
  


}
