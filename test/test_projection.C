#include <iostream>
#include "../lib/rel_operator/project.hpp"
#include "../lib/rel_operator/select_indices.hpp"

using std::cout;
using std::endl;

template <typename ...ColsA, typename ...ColsB>
Table<ColsA...> project(Table<ColsA...> projection, Table<ColsB...> source){
  
   template for (const auto i : std::views::iota(0uz, std::tuple_size_v<decltype(projection.columns)>)) {
      const auto& projection_col = std::get<i>(projection.columns);
      
      template for (const auto y : std::views::iota(0uz, std::tuple_size_v<decltype(source.columns)>)) {
	      cout<< y << endl;
	      const auto& source_col = std::get<y>(source.columns);
	      if( source_col.columnName() == projection_col.columnName()){
		cout << "the columns are equal " <<  source_col.columnName() << endl;
	      }
      }
      
   }
   return projection;
}


int main(void){
  cout << "Working with projections" << endl;
  Table<Column<"id", int>, Column<"name", std::string>, Column<"age", int>> table;
  table.insert_row(1,"Kirill",25);
  table.insert_row(2,"Jenia",26);
  auto result = select<"id">(table, 1);
  Column<"id", int> col;
  col.print_name();
  Table<Column<"id", int>> t1;
  Table<Column<"id", int>> r = project(t1, table);
  // assert(std::get)
  
  
  
}
