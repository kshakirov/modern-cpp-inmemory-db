using namespace std;
#include <iostream>
#include "../lib/rel_operator/rel_operator.hpp"
#include "../lib/lattice_storage/lattice_storage.hpp"




int main(int argc, char** argv){
  cout<< "Emulating our rel engine"<< endl;
  cout<<""<<endl;
  LatticeStorage storage;
  storage.id =1;
  
  Filter(3);
  dump(&storage);
  


}
