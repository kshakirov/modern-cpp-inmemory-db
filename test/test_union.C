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

    constexpr auto l_name = std::get<i>(l_table.columns).columnName();
    constexpr bool name_matches = ((RightCols::columnName() == l_name) || ...);

    if constexpr (!name_matches){
      cout<<"Table columns names do not  match, quitting " <<endl;
      exit(1);
    }
  }

  Table<LeftCols...> result ;
  result.columns = l_table.columns;
  template for (const auto i : std::views::iota(0uz, l_size)){
    std::get<i>(result.columns).column = std::get<i>(l_table.columns).column;
   }

  template for (const auto i : std::views::iota(0uz, l_size)){
    auto& v1 = std::get<i>(result.columns).column;
    auto v2  = std::get<i>(r_table.columns).column;
    v1.insert(v1.begin(),v2.begin(),v2.end());
   }
    
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
  Table<Column<"name", std::string>, Column<"id", int>> l_table;
  Table<Column<"name", std::string>, Column<"id", int>> r_table;
  Table<Column<"name", std::string>, Column<"id", int>, Column<"age", int>> wrong_table;
  l_table.insert_row("Jenia", 1);
  l_table.insert_row("Kirill", 2);
  r_table.insert_row("Stepanov", 3);
  r_table.insert_row("Straustroup", 4);

  //  UnionU1(l_table,wrong_table);
  auto united_tables_of_me = UnionU1(l_table,r_table);

  assert(united_tables_of_me.size() == 2);
  cout << "size "<<  std::get<0>(united_tables_of_me.columns).column.size() << endl;
  assert(std::get<0>(united_tables_of_me.columns).column.size()==4);
}
