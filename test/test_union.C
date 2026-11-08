#include <iostream>
#include <tuple>
#include <ranges>
#include "../lib/lattice_storage/lattice_storage.hpp"


using std::cout;
using std::endl;





// 2 tables for each

template <typename ... LeftCols, typename... RightCols>
auto     union_tables(const Table<LeftCols...>& l_table, const Table<RightCols...>& r_table){

  const auto l_size = std::tuple_size_v<decltype(l_table.columns)>;
  const auto  r_size = std::tuple_size_v<decltype(r_table.columns)>;
  if constexpr (l_size != r_size){
    cout<<"Table columns numbers do not  match, quitting " <<endl;
    exit(1);
  }
  template for (const auto i : std::views::iota(0uz,l_size)){
    const bool name_matches = false;
    const auto l_name = std::get<i>(l_table.columns).columnName();
    template for (const auto y : std::views::iota(0uz,l_size)){
      const auto r_name = std::get<y>(r_table.columns).columnName();
      cout << "l name is " << l_name << " r name is " << r_name << endl;
      if constexpr (l_name == r_name) {
	name_matches = true;
	break;
      }
     }
    if constexpr (!name_matches){
      cout<<"Table columns names do not  match, quitting " <<endl;
    }
  }

  Table<LeftCols...> result ;
   
  return result;
}
//base case one table  table left the template function
template <typename SingleTable>
auto UnionU1(const SingleTable& table){
  return  table;
}

//recurction
template <typename Head, typename ...Rest>
auto UnionU1(const Head& head, const Rest&... tail){
  auto tail_unioned = UnionU1(tail...) ;
  return union_tables(head, tail_unioned);
}
  





int main(void){
  cout << "Working with union" <<endl;
  const Table<Column<"name", std::string>, Column<"id", int>> l_table;
  const Table<Column<"name", std::string>, Column<"id", int>, Column<"age", int>> wrong_table;
  //  UnionU1(l_table,wrong_table);
  UnionU1(l_table,l_table);
}
