#include <iostream>
#include <tuple>
#include <ranges>
#include "../lib/lattice_storage/lattice_storage.hpp"


using std::cout;
using std::endl;





// 2 tables for each

template <typename ... LeftCols, typename... RightCols>
auto     union_two_tables(Table<LeftCols...>, Table<RightCols...>){

  Table<LeftCols...> result ;
  return result;
}
//base caseto tables left
template <typename SingleTable>
auto union_two_table(SingleTable table){
  return  table;
}

//recurction
  





int main(void){
  cout << "Working with union" <<endl;   
}
