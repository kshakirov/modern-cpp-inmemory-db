#include <iostream>
#include "../lib/lattice_storage/lattice_storage.hpp"
using namespace  std;



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


template <FixedString TargetName, typename  T>
struct IsSameName {
  static constexpr bool value = false;
};

template <FixedString TargetName, typename T>
struct IsSameName <TargetName, Column<TargetName, T> >{
  static constexpr bool value = true;
};



template <FixedString TargetName, size_t Index, typename ...Cols>
struct find_column_index{
  static constexpr  size_t value=-1;
};


template <FixedString TargetName, size_t Index, typename T>
struct find_column_index <TargetName, Index, Column<TargetName, T>> { 
  static constexpr  size_t value= IsSameName<TargetName, T>::value ? Index : -1;
};



int main(int argc, char ** argv){
  IsInt<int> isint;
  IsString<std::string> isstring;
  cout<< "Is int"<<isint.value  <<endl;
  cout <<"IsString"<<isstring.value <<endl;
  cout << "Are Equal Types "<< AreEqualTypes<int,int>::value <<endl;
  cout << "Are Names equalt "<< IsSameName<"Name", Column<"id", int>>::value << endl;
  cout << "Are Names equalt "<< IsSameName<"Name", Column<"Name", string>>::value << endl;
  
}
