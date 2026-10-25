#include <iostream>
#include "../lib/rel_operator/project.hpp"
#include "../lib/rel_operator/select_indices.hpp"

using std::cout;
using std::endl;

template <typename ...ColsA, typename ...ColsB>
Table<ColsA...> project(Table<ColsA...> projection, Table<ColsB...> source){
  Table<ColsA...> result;
   template for (const auto i : std::views::iota(0uz, std::tuple_size_v<decltype(projection.columns)>)) {
      const auto& projection_col = std::get<i>(projection.columns);
      
      template for (const auto y : std::views::iota(0uz, std::tuple_size_v<decltype(source.columns)>)) {
	//cout<< y << endl;
	      const auto& source_col = std::get<y>(source.columns); 
	      if constexpr( source_col.columnName() == projection_col.columnName()){
		//cout << "the columns are equal " <<  source_col.columnName() << endl;
		//later		std::is_same<source_col, projection_col>;
		auto cols = source_col.column;
		auto& result_col = std::get<i>(result.columns);
		result_col.column = source_col.column;
	      }
      }
      
   }
   return result;
}


int main(void){
  cout << "Working with projections" << endl;
  Table<Column<"id", int>, Column<"name", std::string>, Column<"age", int>> source_table;
  source_table.insert_row(1,"Kirill",25);
  source_table.insert_row(2,"Jenia",26);
  auto select_result = select<"id">(source_table, 1);
  Column<"id", int> col;
  Table<Column<"id", int>> projection_table;
  auto projection_result  = project(projection_table, source_table);
  assert(std::get<0>(projection_result.columns).columnName() == "id");
  assert(std::get<0>(projection_result.columns).column.size() == std::get<0>(source_table.columns).column.size());
  
  
  
}
