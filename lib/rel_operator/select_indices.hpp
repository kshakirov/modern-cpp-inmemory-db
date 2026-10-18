#pragma once

#include "../lattice_storage/lattice_storage.hpp"

#include <cstddef>
#include <vector>

using RowIndices = std::vector<std::size_t>;

// First stage of relational selection: return the positions of all rows whose
// value in TargetName matches `value`. The algorithm is intentionally left for
// the selection experiment.

template<FixedString TargetName, typename... Cols, typename T>
RowIndices select_indices(const Table<Cols...>& table, const T& val){
  constexpr auto value = find_column_index<TargetName, 0, Cols...>::value;
  const auto& column = std::get<value>(table.columns);
  const auto& col = column.column;
  //    auto it = std::find(col.begin(), col.end(), val);
  RowIndices indices;
  auto it = col.begin();
  while ((it = std::find(it,col.end(),val))!= col.end()) {
    indices.push_back(std::distance(col.begin(),it));
    it++;
  }
  return indices;

}

template<FixedString TargetName, typename... Cols, typename T>
void select(const Table<Cols...>& table , const T& val){
  auto indices = select_indices<TargetName>(table, val);
  Table<Cols...> table2;
  std::cout<<"Here we go bro" << std::endl;
}
