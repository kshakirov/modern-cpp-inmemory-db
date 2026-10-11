#include "../lib/rel_operator/select_indices.hpp"
using std::cout;
using std::endl;
#include <cassert>

int main() {
  Table<Column<"id", int>, Column<"name", std::string>> table;
  table.insert_row(1, "A");
  table.insert_row(2, "B");
  table.insert_row(1, "A");
  table.insert_row(3, "C");

  const auto& id_column = std::get<0>(table.columns).column;
  assert((id_column == std::vector<int>{1, 2, 1, 3}));
  auto result =  select_indices<"id">(table, 1);
  assert(result.at(0)==0);
  assert(result.at(1)==2);
  //  cout << "the size of the result is "<< result.size() << endl;
  assert(result.size() == 2);
  auto no_result = select_indices<"id">(table, 101);
  assert(no_result.size() == 0);
  auto one_result = select_indices<"id">(table, 3);
  assert(one_result.size() == 1);
  // TODO: no match    -> []
  // TODO: one match   -> [1]
  // TODO: many matches -> [0, 2]
}
