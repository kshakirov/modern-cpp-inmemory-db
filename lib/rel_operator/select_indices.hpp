#pragma once

#include "../lattice_storage/lattice_storage.hpp"

#include <cstddef>
#include <vector>

using RowIndices = std::vector<std::size_t>;

// First stage of relational selection: return the positions of all rows whose
// value in TargetName matches `value`. The algorithm is intentionally left for
// the selection experiment.
template<FixedString TargetName, typename... Cols, typename T>
RowIndices select_indices(const Table<Cols...>& table, const T& value);
