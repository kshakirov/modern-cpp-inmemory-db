#include "../lib/rel_operator/select_indices.hpp"
#include <cassert>

int main() {
  Table<Column<"id", int>, Column<"name", std::string>> table;
  table.insert_row(1, "A");
  table.insert_row(2, "B");
  table.insert_row(1, "A");
  table.insert_row(3, "C");

  const auto& id_column = std::get<0>(table.columns).column;
  assert((id_column == std::vector<int>{1, 2, 1, 3}));
  auto result = select_indices<"id">(table, 1);
  assert(result.at(0)==0);
  assert(result.at(1)==2);
  assert(result.size() == 2);
  auto no_result = select_indices<"id">(table, 101);
  assert(no_result.size() == 0);
  auto one_result = select_indices<"id">(table, 3);
  assert(one_result.size() == 1);

  auto result_table = select<"id">(table, 1);
  assert((std::get<0>(result_table.columns).column == std::vector<int>{1, 1}));
  assert((std::get<1>(result_table.columns).column == std::vector<std::string>{"A", "A"}));

  auto one_row_table = select<"id">(table, 3);
  assert((std::get<0>(one_row_table.columns).column == std::vector<int>{3}));
  assert((std::get<1>(one_row_table.columns).column == std::vector<std::string>{"C"}));

  auto empty_table = select<"id">(table, 101);
  assert(std::get<0>(empty_table.columns).column.empty());
  assert(std::get<1>(empty_table.columns).column.empty());
}
