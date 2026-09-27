#include <iostream>
#include <tuple>
#include <ranges>
#include "../lib/lattice_storage/lattice_storage.hpp"
//using namespace  std;

using std::string;
using std::cout;
using std::endl;


template <typename A, typename B, FixedString aname, FixedString bname>
Table<Column< aname, A >,Column<bname, B>> Join(Table<Column< aname, A>> tablea, Table<Column<bname,B >> tableb){
  //create copies of   on purpose
  auto& colsa = tablea.columns;
  auto& colsb = tableb.columns;

  auto& cola = std::get<0>(colsa);
  auto& colb = std::get<0>(colsb);
  std::cout << "size of cola is " << cola.column.size() << " size of colb is "<< colb.column.size()<< endl;
  
  constexpr size_t n = std::tuple_size_v<decltype(tablea.columns)>;  // 3
  constexpr size_t n2 = std::tuple_size_v<decltype(tableb.columns)>;  // 3
  
  std::cout << "size of tablea tuple  is " << n  << " size of tableb is "<< n2<< endl;
  Table<Column<aname, A>,Column<bname, B> > table;
  //lets pretend we hava only one column per table get size of join table
  size_t common_size = cola.column.size() > colb.column.size() * colb.column.size();
  //init 2 vectors of this size for each column
  std::vector<A> vec_a(common_size);
  std::vector<B> vec_b(common_size);
  
  for (auto v1: cola.column){
    for( auto v2: colb.column){
      vec_a.push_back(v1);
      vec_b.push_back(v2);
    }
  }

  //create new columns
  Column<aname, A> new_cola;
  Column<bname, B> new_colb;

  new_cola.column = vec_a;
  new_colb.column = vec_b;
  
  
  
  std::get<0>(table.columns) = new_cola;
  std::get<1>(table.columns) = new_colb;
  

  //  table.columns = std::tuple_cat(colsa,colsb);

  return  table;   
  
  
  
}

int main(void){
  cout<<"Trying to get Cartesian product to work" <<endl;
  Table<Column<"id",int>> table1 ;
  Table<Column<"name", std::string>>  table2;
  table1.insert_row(1);
  for   (auto i : std::views::iota(0, 5)){

    table2.insert_row( std::string("kirill_") + std::to_string(toascii(i + 64))) ;
  }
  auto table_join = Join(table1,table2);
  
  auto r = table_join.get_index<"id">(1);
  
  cout<< " result is " << r << " " <<endl;
  assert(r==0);

  auto& col_id = std::get<0>(table_join.columns);
  auto& col_name = std::get<1>(table_join.columns);

  assert(col_id.column.size()==5);
  assert(col_name.column.size()==5);
  
  
  //  Join(Table<Column<"namea", A>>, Table<Column<"nameb", B>>)
}
