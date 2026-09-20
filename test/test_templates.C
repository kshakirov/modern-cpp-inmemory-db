#include <iostream>
#include "../lib/lattice_storage/lattice_storage.hpp"
//using namespace  std;

using std::string;
using std::cout;
using std::endl;
//these are the experiments

template<typename  T>
struct IsInt {
  static constexpr bool value= false;
};
template<>
struct IsInt<int>{
  static constexpr bool value = true;
};

template<typename  T>
struct IsString {
  static constexpr bool value= false;
};
template<>
struct IsString<std::string>{
  static constexpr bool value = true;
};


template <typename T1, typename T2>
struct AreEqualTypes {
  static constexpr bool value = false;
};

template <typename T>
struct AreEqualTypes<T,T> {
  static constexpr bool value = true;
};




int main(int argc, char ** argv){
  IsInt<int> isint;
  IsString<std::string> isstring;
  cout<< "Is int"<<isint.value  <<endl;
  cout <<"IsString"<<isstring.value <<endl;
  cout << "Are Equal Types "<< AreEqualTypes<int,int>::value <<endl;
  cout << "Are Names equal "<< IsSameName<"Name", Column<"id", int>>::value << endl;
  cout << "Are Names equal "<< IsSameName<"Name", Column<"Name", string>>::value << endl;
  Table<Column<"id", int>, Column<"name", string>> table;

  table.insert_row(2,"Tamanzev");
  table.insert_row(1,"Alehin");
  find_column_index<"id", 1, Table<Column<"id", int>, Column<"name", string>>> find_col_ind;
  auto r = table.get_index<"id">(1);
  cout<<"Found " << r << " result" << endl;
  assert(r == 1);
  
  using TestColumns = find_column_index<
    "name",
    0,
    Column<"id", int>,
    Column<"name", std::string>,
    Column<"age", int>
    >;

  static_assert(TestColumns::value == 1);
  static_assert(
		find_column_index<
		"missing",
		0,
		Column<"id", int>,
		Column<"name", std::string>
		>::value == static_cast<std::size_t>(-1)
		);

  

}
