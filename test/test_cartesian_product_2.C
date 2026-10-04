#include <iostream>
#include <tuple>
#include <ranges>
#include "../lib/lattice_storage/lattice_storage.hpp"
//using namespace  std;

using std::string;
using std::cout;
using std::endl;






template <typename ...Cols, typename Head, FixedString aname>
Table<Cols... > add_column(Column<aname, Head> column, Table<Cols...> table){

  constexpr std::size_t tuple_size =    std::tuple_size_v<decltype(table.columns)>;
  std::get<tuple_size>(table.columns) = column;
  return table;

};

template<typename ...Cols, typename  Head, typename  ...Tail, FixedString aname, int index>

Table<Column<aname, Head> , Tail...> JoinC1(Table<Column<aname, Head>>table, Tail... tt  ){
  auto& col = table.columns;
  auto& cola = std::get<0>(col);
    
  //    Table<Column<aname, Head> , Tail...> t;
  Column<aname, Head> new_head_column;
  new_head_column.column = cola.column;
  //std::get<index>(t.columns) = new_head;
  return add_column(new_head_column, JoinC1(tt...));
    

  //  constexpr static int  counter = counter + 1;
}

template <typename ...Cols>
Table<Cols...> JoinC1(){
  Table<Cols...> table;
  return table;
  
}



int main(void){
  cout<<"Trying to get Cartesian product to work" <<endl;
  Table<Column<"id",int>> table1 ;
  Table<Column<"name", std::string>>  table2;
  table1.insert_row(1);
  for   (auto i : std::views::iota(0, 5)){

    table2.insert_row( std::string("kirill_") + std::to_string(toascii(i + 64))) ;
  }
  //   auto table_join = JoinC1(table1,table2);
  
  // auto r = table_join.get_index<"id">(1);
  
  // cout<< " result is " << r << " " <<endl;
  // assert(r==0);

  // auto& col_id = std::get<0>(table_join.columns);
  // auto& col_name = std::get<1>(table_join.columns);

  // assert(col_id.column.size()==5);
  // assert(col_name.column.size()==5);
  
  
  //  Join(Table<Column<"namea", A>>, Table<Column<"nameb", B>>)
}
