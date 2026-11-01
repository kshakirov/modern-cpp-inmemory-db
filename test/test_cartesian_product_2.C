#include <iostream>
#include <tuple>
#include <ranges>
#include "../lib/lattice_storage/lattice_storage.hpp"
//using namespace  std;

using std::string;
using std::cout;
using std::endl;



// =================================================================
// 1. БАЗОВЫЙ ШАГ: Склейка ДВУХ таблиц в одну (Вариант 1)
// =================================================================
template <typename... LeftCols, typename... RightCols>
auto join_two_tables(const Table<LeftCols...>& t1,
                     const Table<RightCols...>& t2) {
  Table<LeftCols..., RightCols...> result;
  // Склеиваем кортежи с колонками в новый tuple
  cout << "Joining columns" << endl;
  //lresult.columns = std::tuple_cat(t1.columns, t2.columns);
  
  const size_t i_size = std::tuple_size_v<decltype(t1.columns)>;
  const size_t j_size = std::tuple_size_v<decltype(t2.columns)>;
  template for (const auto i : std::views::iota(0uz, std::tuple_size_v<decltype(t1.columns)>)) {
    auto& t1_copy = std::get<i>(result.columns).column;
    for (auto v : (std::get<i>(t1.columns)).column){
      cout <<"External cycle " <<v << endl;
      template for (const auto j : std::views::iota(0uz, std::tuple_size_v<decltype(t2.columns)>)) {
	auto& t2_copy = std::get<i_size + j>(result.columns).column;
      for (auto w : (std::get<j>(t2.columns)).column){
	cout <<"Internal cycle adding v = " << v<< " w = " << w << endl;
	if(i==0){
	  t2_copy.push_back(w);
	}
	if(j==0){
	  t1_copy.push_back(v);
	}
      }
    }
    }

  }
    
	
  return result;
}

// =================================================================
// 2. РЕКУРСИВНЫЙ ШАГ: Склейка N таблиц (Вариант 2 - JoinC1)
// =================================================================

// База рекурсии: если осталась всего одна таблица — просто возвращаем её
template <typename  SingleTable>
auto JoinC1(SingleTable table) {
  return table;
}

// Шаг рекурсии: берем первую таблицу (head) и склеиваем с результатом JoinC1 для остальных (tail...)
template <typename HeadTable, typename ...TailTables>
auto JoinC1(HeadTable head, TailTables... tail) {
  // 1. Рекурсивно склеиваем все оставшиеся таблицы
  auto tail_joined = JoinC1(tail...);

  // 2. Склеиваем первую таблицу с оставшимся куском через наш Базовый Шаг
  return join_two_tables(head, tail_joined);
}


int main(void){
  cout<<"Trying to get Cartesian product to work" <<endl;
  Table<Column<"id",int>> table1 ;
  Table<Column<"name", std::string>, Column<"age", int>>  table2;
  table1.insert_row(1);
  table1.insert_row(2);
  for   (auto i : std::views::iota(0, 5)){

    table2.insert_row( std::string("kirill_") + std::to_string(toascii(i + 64)), i) ;
  }
  auto table_join = JoinC1(table1,table2);
  assert(std::tuple_size_v<decltype(table1.columns)> ==1);
  assert(std::tuple_size_v<decltype(table2.columns)> ==2);
  assert(std::get<0>(table1.columns).column.size()==2);
  assert(std::get<0>(table2.columns).column.size()==5);
  assert(std::get<1>(table2.columns).column.size()==5);
  assert(std::tuple_size_v<decltype(table_join.columns)> ==3);

  cout << "size of " << 1 << "is " <<  std::get<0>(table_join.columns).column.size()<< endl;
  cout << "size of " << 2 << "is " <<  std::get<1>(table_join.columns).column.size()<< endl;
  cout << "size of " << 3 << "is " <<  std::get<2>(table_join.columns).column.size()<< endl;
  assert(std::get<0>(table_join.columns).column.size()==10);
  assert(std::get<1>(table_join.columns).column.size()==10);
  assert(std::get<2>(table_join.columns).column.size()==10);
}
