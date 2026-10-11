#include "../lib/rel_operator/select_indices.hpp"

#include <cassert>

int main() {
  Table<Column<"id", int>> table;
  table.insert_row(1);
  table.insert_row(2);
  table.insert_row(1);
  table.insert_row(3);

  const auto& id_column = std::get<0>(table.columns).column;
  assert((id_column == std::vector<int>{1, 2, 1, 3}));

  // TODO: no match    -> []
  // TODO: one match   -> [1]
  // TODO: many matches -> [0, 2]
}
