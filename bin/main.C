using namespace std;
#include <iostream>
#include "../lib/rel_operator/rel_operator.hpp"
#include "../lib/lattice_storage/lattice_storage.hpp"
#include <iostream>
#include <vector>
#include <generator> // Требуе
#include <algorithm>

// 1. Тот самый структурный тип-массив
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
  void print_name() {
    // Мы можем напечатать имя, потому что байты сохранены в массиве!
    std::cout << "Column name: " << Name.data << std::endl;
  }
};


int main(int argc, char** argv){
  cout<< "Emulating our rel engine"<< endl;
  cout<<""<<endl;
  LatticeStorage storage;
  storage.id =1;
  Column<"id", int> id_column;
  id_column.print_name(); // Выведет: Column name: id
  Filter(&storage, 3);
  dump(&storage);
  


}
