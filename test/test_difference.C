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
  if constexpr (l_size == r_size){
    cout<<"Table columns numbers do not  match, quitting " <<endl;
    //    exit(1);
  }else {
    static_assert(false, "columns numbers  must match");
  }
  template for (const auto i : std::views::iota(0uz,l_size)){

    constexpr auto l_name = std::get<i>(l_table.columns).columnName();
    constexpr auto r_name = std::get<i>(r_table.columns).columnName();

    if constexpr (l_name == r_name){
      cout<<"Table columns names   match, quitting " <<endl;
      //exit(1);
    }else{
      static_assert(false, "both columns must be of the same type");
    }
  }

  Table<LeftCols...> result ;
  result.columns = l_table.columns;
  template for (const auto i : std::views::iota(0uz, l_size)){
    std::get<i>(result.columns).column = std::get<i>(l_table.columns).column;
   }

  template for (const auto i : std::views::iota(0uz, l_size)){
    
    auto& v1 = std::get<i>(result.columns).column;
    auto& v2  = std::get<i>(r_table.columns).column;
    using V1 = std::remove_reference_t<decltype(v1)>;
    using V2 = std::remove_reference_t<decltype(v2)>;
    if constexpr (std::is_same_v<typename V1::value_type, typename V2::value_type>) {
      v1.insert(v1.begin(),v2.begin(),v2.end());
    // типы совпадают
    }else{
      static_assert(false, "both columns must be of the same type");
    }

   }
    
  return result;
}
//base case one table  table left the template function
template <typename SingleTable>
auto DifferenceD1(const SingleTable& table){
  return  table;
}

//recurction
template <typename Head, typename ...Rest>
auto DifferenceD1(const Head& head, const Rest&... tail){
  auto tail_unioned = DifferenceD1(tail...) ;
  return union_tables(head, tail_unioned);
}
  





int main(void){
  cout << "Working with difference" <<endl;
  Table<Column<"name", std::string>, Column<"id", int>> l_table;
  Table<Column<"name", std::string>, Column<"id", int>> r_table;
  Table<Column<"name", std::string>, Column<"id", int>, Column<"age", int>> wrong_table_size;
  Table<Column<"name", std::string>, Column<"id", std::string>> wrong_table_type;
  l_table.insert_row("Jenia", 1);
  l_table.insert_row("Kirill", 2);
  r_table.insert_row("Stepanov", 3);
  r_table.insert_row("Straustroup", 4);
  wrong_table_size.insert_row("Petya",5,27);
  wrong_table_type.insert_row("no name", "6");
  
  //  DifferenceD1(l_table,wrong_table);
  auto united_tables_of_me = DifferenceD1(l_table,r_table);

  assert(united_tables_of_me.size() == 2);
  cout << "size "<<  std::get<0>(united_tables_of_me.columns).column.size() << endl;
  assert(std::get<0>(united_tables_of_me.columns).column.size()==4);


  //   auto wrongly_united_tables = DifferenceD1(l_table, wrong_table_size);
  //  auto wrongly_united_tables = DifferenceD1(l_table, wrong_table_type);
}
