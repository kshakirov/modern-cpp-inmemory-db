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
  auto result =  table.get_indices<"id">(1);
  cout << "the size of the result is "<< result.size() << endl;
  assert(result.size() == 2);
  // TODO: no match    -> []
  // TODO: one match   -> [1]
  // TODO: many matches -> [0, 2]
}
