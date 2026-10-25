#include <iostream>
#include "../lib/rel_operator/project.hpp"
#include "../lib/rel_operator/select_indices.hpp"

using std::cout;
using std::endl;

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
  assert((std::get<0>(projection_result.columns).column == std::vector<int>{1, 2}));

  Table<Column<"age", int>, Column<"name", std::string>> reordered_schema;
  auto reordered_result = project(reordered_schema, source_table);
  assert((std::get<0>(reordered_result.columns).column == std::vector<int>{25, 26}
       && std::get<1>(reordered_result.columns).column == std::vector<std::string>{"Kirill", "Jenia"}));
  
  
  
}
