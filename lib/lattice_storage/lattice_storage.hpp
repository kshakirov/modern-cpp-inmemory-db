#include "../all_types.hpp"
#include <algorithm>
#include <vector>
#include <optional>

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
  std::optional<T> select_one(const T val){
    auto it =  std::find(column.begin(), column.end(), val);
    if (it != column.end()) {
      return *it;
    } else {
      return  std::nullopt;
    }
  }
 
};


void dump(LatticeStorage* storage );
