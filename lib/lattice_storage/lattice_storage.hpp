#include "../all_types.hpp"
#include <algorithm>
#include <tuple>
#include <vector>
#include <optional>
#include <assert.h>
template<std::size_t N>
struct FixedString {
  char data[N]{}; // Открытый массив символов

  // 2. Элемент магии: constexpr конструктор "ловит" указатель-литерал
  // и прямо во время компиляции копирует его в массив data
  constexpr FixedString(const char (&str)[N]) {
    std::copy_n(str, N, data);
  }
};

// 3. Шаблон колонки, принимающий НАШ новый тип как значение
template<FixedString Name, typename T>
struct Column {
  std::vector<T > column;
  void print_name() {
    // Мы можем напечатать имя, потому что байты сохранены в массиве!
    std::cout << "Column name: " << Name.data << std::endl;
  };
  int  insert (T val){
    column.push_back(val);
    return  1;
  }
  std::optional<T> select_one(const T& val){
    auto it =  std::find(column.begin(), column.end(), val);
    if (it != column.end()) {
      return *it;
    } else {
      return  std::nullopt;
    }
  }
    
};


// template <FixedString TargetName, typename  T>
// struct IsSameName {
//   static constexpr bool value = false;
// };

// template <FixedString TargetName, typename T>
// struct IsSameName <TargetName, Column<TargetName, T> >{
//   static constexpr bool value = true;
// };






template <FixedString TargetName, typename  T>
struct IsSameName {
  static constexpr bool value = false;
};

template <FixedString TargetName, typename T>
struct IsSameName <TargetName, Column<TargetName, T> >{
  static constexpr bool value = true;
};


template <FixedString TargetName, size_t Index, typename ...Cols>
struct find_column_index;

template <FixedString TargetName, size_t Index, typename FirstCol, typename  ... RestCols>
struct find_column_index<TargetName, Index, FirstCol, RestCols...>{
  static constexpr  size_t value= IsSameName<TargetName, FirstCol>::value ?
    Index :
    find_column_index<TargetName, Index + 1, RestCols...>::value;
};


template <FixedString TargetName, size_t Index>
struct find_column_index <TargetName, Index> { 
  static constexpr  size_t value= -1;
};

template<typename... Cols>
struct Table {
  std::tuple<Cols...> columns;
  
  template<typename... Args>
  void insert_row(Args&&... args) {
    std::apply([&](auto&... cols) {
      ((cols.insert(std::forward<Args>(args))), ...);
    }, columns);

  };
  template<FixedString TargetName, typename  T>
  size_t  get_index(const T& val){
    constexpr auto value = find_column_index<TargetName, 0, Cols...>::value;
    const auto& column = std::get<value>(columns);
    const auto& col = column.column;
    auto it = std::find(col.begin(), col.end(), val);
    if(it == col.end()){
      return -1;
    }else{
      return std::distance(col.begin(), it);
    }
    
      
    }
    
   
    
   
};   
  


void dump(LatticeStorage* storage );
