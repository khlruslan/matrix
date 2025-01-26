#include <iostream>
#include <cassert>
#include <utility>

#include "matrix.h"
#include "trace_help.h"

void unit_tests(){
  {
    dd::Say say("Simple constructor test");

    mm::Matrix<int> m(10,10, 1);
    m.dump(std::cout);
  }
  {
    dd::Say say("Iterator constructor test");
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    mm::Matrix<int> m(3, 3, a, a + (sizeof(a) / sizeof(*a)));
    m.dump(std::cout);
  }
  {
    dd::Say say("Eye method test");
    auto m = mm::Matrix<int>::eye(10,10, 1);
    m.dump(std::cout);
  }
  {
    dd::Say say(" operator [][] test");
    auto m = mm::Matrix<int>::eye(10,10, 1);
    m[2][1] = 4;
    m.dump(std::cout);
  }
  {
    dd::Say say(" Copy constructor test");
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    mm::Matrix<int> m(3, 3, a, a + (sizeof(a) / sizeof(*a)));
    m[0][2] = 10;
    auto m_copy = m;
    m_copy.dump(std::cout);
  }
  {
    dd::Say say("Move constructor test");
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    mm::Matrix<int> m(3, 3, a, a + (sizeof(a) / sizeof(*a)));
    m[0][2] = 10;
    m.dump(std::cout);
    auto m_copy(std::move (m));
    m_copy.dump(std::cout);
    m.dump(std::cout);
  }
  {
    dd::Say say("Copy assigment test");
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    mm::Matrix<int> m(3, 3, a, a + (sizeof(a) / sizeof(*a)));
    m.dump(std::cout);
    m[0][2] = 10;
    mm::Matrix<int> m2(10,10,5);
    m2.dump(std::cout);
    m2 = m;
    m2.dump(std::cout);
  }
  {
    dd::Say say("Move assigment test");
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    mm::Matrix<int> m(3, 3, a, a + (sizeof(a) / sizeof(*a)));
    m[0][2] = 10;
    m.dump(std::cout);

    mm::Matrix<int> m2(10,10,5);
    m2.dump(std::cout);
    m2 = std::move(m);
    m2.dump(std::cout);
    m.dump(std::cout);
  }
  {
    dd::Say say("Swap rows method test");
    auto m = mm::Matrix<int>::eye(10,10, 1);
    m.swap_rows(2, 5);
    m.dump(std::cout);
  }
  {
    dd::Say say("Swap columns method test");
    auto m = mm::Matrix<int>::eye(10,10, 1);
    m.swap_columns(3, 7);
    m.dump(std::cout);
  }
  {
    dd::Say say("trace method test");
    auto m = mm::Matrix<int>(10,10, 5);
    m.dump(std::cout);
    int t = m.trace();
    std::cout << "Trace :" << t << '\n';
    assert(t == 50); 
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    mm::Matrix<int> m2(3, 3, a, a + (sizeof(a) / sizeof(*a)));
    m2.dump(std::cout);
    t = m2.trace();
    std::cout << "Trace :" << t << '\n';
    assert(t == 15); 
  }
  {
    dd::Say say("Equal  method test");
    auto m = mm::Matrix<int>::eye(10,10, 1);
    m.swap_columns(3, 7);
    m.dump(std::cout);
    auto m2 = m;
    m2.dump(std::cout);
    assert(m.equal(m2));
    m.swap_columns(1, 5);
    assert(!m.equal(m2));
  }
  {
    dd::Say say("Less  method test");
    mm::Matrix<int> m(10,10, 1);
    mm::Matrix<int> m2(10,10, 2);
    assert(m.less(m2));
    assert(!m2.less(m));
  }
  {
    dd::Say say("Negate method test");
    mm::Matrix<int> m(10,10, 16);
    m.negate();
    m.dump(std::cout);
  }
  {
    dd::Say say("Transponse constructor test");
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    mm::Matrix<int> m(3, 4, a, a + (sizeof(a) / sizeof(*a)));
    m.dump(std::cout);
    m.transpose();
    m.dump(std::cout);
  }
  {
    dd::Say say("Determinant test");
    int a[] = {3, 2, 3, 4, 4, 4, 3, 2, 1, 4, 4, 3, 2, 3, 1, 1};
    mm::Matrix<int> m(4, 4, a, a + (sizeof(a) / sizeof(*a)));
    m.dump(std::cout);
    const int result = m.BareissDeterminant();
    std::cout << "Determinant :" << result << '\n';
    assert (45 == result);
  }
  {
    dd::Say say("Random method test");
    const int determinant = 42;
    auto m = mm::Matrix<int>::random(100, determinant, 1 , 2 * determinant );
    m.dump(std::cout);
    const int result = m.BareissDeterminant();
    std::cout << "Determinant :" << result << '\n';
    assert (determinant == result);
  }
}

int random_test(){
  const int determinant = 42;
  auto m = mm::Matrix<int>::random(1000, determinant, 1 , 1000 * determinant );
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
#ifdef my_debug
  unit_tests();
#endif
  return random_test();
}
