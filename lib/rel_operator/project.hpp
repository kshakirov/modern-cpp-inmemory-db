#pragma once
#include "../lattice_storage/lattice_storage.hpp"
#include <cstddef>
#include <ranges>
#include <tuple>

// The first table supplies the result schema; its data is not used.
// Requested names must exist in the source with matching value types.
template <typename... ColsA, typename... ColsB>
Table<ColsA...> project(const Table<ColsA...>& projection,
                       const Table<ColsB...>& source) {
  Table<ColsA...> result;
  template for (const auto i : std::views::iota(0uz, std::tuple_size_v<decltype(projection.columns)>)) {
    const auto& projection_col = std::get<i>(projection.columns);
    template for (const auto y : std::views::iota(0uz, std::tuple_size_v<decltype(source.columns)>)) {
      const auto& source_col = std::get<y>(source.columns);
      if constexpr (source_col.columnName() == projection_col.columnName()) {
        auto& result_col = std::get<i>(result.columns);
        result_col.column = source_col.column;
      }
    }
  }
  return result;
}
