#include <iostream>
#include <tuple>
#include "../lib/lattice_storage/lattice_storage.hpp"
//using namespace  std;

using std::string;
using std::cout;
using std::endl;


template <typename A, typename B, FixedString aname, FixedString bname>
Table<Column< aname, A >,Column<bname, B>> Join(Table<Column< aname, A>> tablea, Table<Column<bname,B >> tableb){
  auto& colsa = tablea.columns;
  auto& colsb = tableb.columns;
  auto& cola = std::get<0>(colsa);
  auto& colb = std::get<0>(colsb);

  Table<Column<aname, A>,Column<bname, B> > table;

  table.columns = std::tuple_cat(colsa,colsb);
  return  table;   
  
  
  
}

int main(void){
  cout<<"Trying to get Cartesian product to work" <<endl;
  Table<Column<"id",int>> table1 ;
  table1.insert_row(1);
  Table<Column<"name", std::string>>  table2;
  table2.insert_row("kirill");
  auto table_join = Join(table1,table2);
  
  auto r = table_join.get_index<"id">(1);
  cout<< " result is " << r << " " <<endl;
  assert(r==0);
  
  //  Join(Table<Column<"namea", A>>, Table<Column<"nameb", B>>)
}
