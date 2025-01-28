#include "matrix.h"

#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace my {
  namespace project {
    namespace {

      template <typename T>
        std::vector<T> GetVectorFromMatrix(const mm::Matrix<T>& m){
          int length = m.nrows() * m.ncols();
          std::vector<T> result;
          result.reserve(length);
          for (int i = 0, ie = m.nrows(); i < ie ; ++i){
            for (int j = 0, je = m.ncols(); j < je ; ++j){
              result.push_back(m[i][j]);
            }
          }
          return result;
        }

      // Tests that the matrix constructor creates 10x10 marix of 1s
      TEST(MatrixInt, Constructor) {
        mm::Matrix<int> m(10,10, 1);
        auto result = GetVectorFromMatrix(m);
        std::vector<int> required(100,1);
        EXPECT_EQ(result, required);
      }

      TEST(MatrixInt, Constructor12As3x4) {
        std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        const mm::Matrix<int> m(3, 4, a.begin(), a.end());
        EXPECT_EQ(m.nrows(), 4);
        EXPECT_EQ(m.ncols(), 3);
        EXPECT_EQ(m[1][0], 4);
        EXPECT_EQ(m[3][0], 10);
        auto result = GetVectorFromMatrix(m);
        EXPECT_EQ(result, a);
      }

      TEST(MatrixInt, Eye) {
        std::vector<int> a = {1, 0, 0, 0, 1, 0, 0, 0, 1};
        auto m =  mm::Matrix<int>::eye(3, 3, 1);
        EXPECT_EQ(m.nrows(), 3);
        EXPECT_EQ(m.ncols(), 3);
        EXPECT_EQ(m[0][0], 1);
        EXPECT_EQ(m[1][1], 1);
        EXPECT_EQ(m[2][2], 1);
        auto result = GetVectorFromMatrix(m);
        EXPECT_EQ(result, a);
      }

      TEST(MatrixInt, CopyConstructor) {
        std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        mm::Matrix<int> m(3, 3, a.begin(), a.end());
        m[0][2] = 10;
        const auto m_copy = m;
        a[2] = m[0][2];
        auto result = GetVectorFromMatrix(m_copy);
        EXPECT_EQ(result, a);
      }

      TEST(MatrixInt, MoveConstructor) {
        std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        mm::Matrix<int> m(3, 3, a.begin(), a.end());
        auto ptr = &(m[0][0]);
        const auto m_copy(std::move(m));
        auto result = GetVectorFromMatrix(m_copy);
        EXPECT_EQ(result, a);
        EXPECT_EQ(m.nrows(), 0);
        EXPECT_EQ(m.ncols(), 0);
        EXPECT_EQ(ptr, &m_copy[0][0]);
      }

      TEST(MatrixInt, CopyAssignment) {
        std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        mm::Matrix<int> m(4, 3, a.begin(), a.end());
        mm::Matrix<int> m2(10, 10, 30);
        m2 = m;
        auto result = GetVectorFromMatrix(m2);
        EXPECT_EQ(result, a);
        EXPECT_EQ(m2.nrows(), 3);
        EXPECT_EQ(m.ncols(), 4);
      }

      TEST(MatrixInt, MoveAssignment) {
        std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        mm::Matrix<int> m(3, 3, a.begin(), a.end());
        auto ptr = &(m[0][0]);
        mm::Matrix<int> m2(10, 10, 30);
        m2 = std::move(m);
        auto result = GetVectorFromMatrix(m2);
        EXPECT_EQ(result, a);
        EXPECT_EQ(m.nrows(), 10);
        EXPECT_EQ(m.ncols(), 10);
        EXPECT_EQ(ptr, &m2[0][0]);
      }

      TEST(MatrixInt, SwapRows) {
        std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        std::vector<int> b = {9, 10, 11, 12, 5, 6, 7, 8, 1, 2, 3, 4};
        mm::Matrix<int> m(4, 3, a.begin(), a.end());
        m.swap_rows(0, 2);
        auto result = GetVectorFromMatrix(m);
        EXPECT_EQ(result, b);
        EXPECT_EQ(m.nrows(), 3);
        EXPECT_EQ(m.ncols(), 4);
      }

      TEST(MatrixInt, SwapColumns) {
        std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        std::vector<int> b = {4, 2, 3, 1, 8, 6, 7, 5, 12, 10, 11, 9};
        mm::Matrix<int> m(4, 3, a.begin(), a.end());
        m.swap_columns(0, 3);
        auto result = GetVectorFromMatrix(m);
        EXPECT_EQ(result, b);
        EXPECT_EQ(m.nrows(), 3);
        EXPECT_EQ(m.ncols(), 4);
      }

      TEST(MatrixInt, Trace) {
        std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector<int> b = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        mm::Matrix<int> m(3, 3, a.begin(), a.end());
        mm::Matrix<int> m2(3, 3, a.begin(), a.end());
        mm::Matrix<int> m3(3, 4, b.begin(), b.end());
        EXPECT_EQ(m.equal(m2), true);
        EXPECT_EQ(m.equal(m3), false);
        EXPECT_EQ(m2.equal(m), true);
        EXPECT_EQ(m3.equal(m), false);
        EXPECT_EQ(m2.equal(m3), false);
        EXPECT_EQ(m3.equal(m2), false);
      }

      TEST(MatrixInt, BareisDeterminant4x4) {
        std::vector<int> a = {3, 2, 3, 4, 4, 4, 3, 2, 1, 4, 4, 3, 2, 3, 1, 1};
        mm::Matrix<int> m(4, 4, a.begin(), a.end());
        EXPECT_EQ(45, m.BareissDeterminant());
      }

      TEST(MatrixInt, BareisDeterminant100x100Random) {
        const int determinant = 42;
        auto m = mm::Matrix<int>::random(100, determinant, 1 , 2 * determinant );
        EXPECT_EQ(determinant, m.BareissDeterminant());
      }

      TEST(MatrixInt, BareisDeterminant1000x1000Random) {
        const int determinant = 421;
        auto m = mm::Matrix<int>::random(1000, determinant, 1 , 2 * determinant );
        EXPECT_EQ(determinant, m.BareissDeterminant());
      }

    }  // namespace
  }  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
