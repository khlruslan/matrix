#include <iostream>
#include <cassert>
#include <utility>

#include "matrix.h"
#include "trace_help.h"

int random_test(){
  const int determinant = 42;
  auto m = mm::Matrix<int>::random(1000, determinant, 1 , 2 * determinant );
  const int result = m.BareissDeterminant();
  std::cout << "Determinant :" << result << '\n';
  if (result == determinant){
    std::cout << "OK \n";
    return 0;
  }else{
    std::cout << "Fail \n";
    return 1;
  }
}

int main(){
  std::cout << "Determinantr calculation by Bareiss method for Matrix<int> 1000x1000  (v1.0)\n";
  return random_test();
}
