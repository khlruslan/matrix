#pragma once
#include <iostream>
#include <iterator>
#include <algorithm>
#include <iterator>
#include <utility>
#include <random>
#include <stdexcept>
#include <cassert>


namespace mm{

    template<class T> 
      bool m_abs(const T& a){
        return a < T{0} ? -a : a ;
      }

  template <class T>
    class Matrix{
      int cols_ = {};
      int rows_ = {};

      T **data_ = nullptr;

      private:
      struct ProxyRow {
        T *row;
        const T& operator[](int n) const { return row[n]; }
        T& operator[](int n) { return row[n]; }
      };

      // представление
       static T** create_data(int cols, int rows, T val = T{} );
       static T** copy_data(int cols, int rows, T** data);
       static void free_data(Matrix& m);
       static void free_data(int cols, int rows, T** data);
       static void fill_row(T *row, int cols,  T val = T{});

      public:
      // конструктор для создания матрицы, заполненной значением
      Matrix(int cols, int rows, T val = T{});
      // конструктор для создания из заданной последовательности
      template <typename It>
        Matrix(int cols, int rows, It start, It fin);
      // "конструктор" для создания единичной матрицы
      static Matrix eye(int n, int m, T val = T{});
      // "конструктор" для создания случайной матрицы
      static Matrix random(int n, T determinant, T low , T high);
      // копирующий и перемещающий конструктор
      Matrix(const Matrix &rhs);
      Matrix(Matrix &&rhs) noexcept;
      // присваивание и перемещение
      Matrix& operator=(const Matrix &rhs);
      Matrix& operator=(Matrix &&rhs) noexcept;
      // деструктор
      ~Matrix();


      // базовые
      int ncols() const;
      int nrows() const;
      // агрегатные
      // след матрицы - сумма главной диагонали
      T trace() const;
      // равенство
      bool equal(const Matrix& other) const;
      // меньше чем other 
      bool less(const Matrix& other) const;
      void dump(std::ostream& os) const;
      // отрицание
      Matrix& negate() &;
      // почему не Matrix transpose() const?
      Matrix& transpose() &;

      // обмен строк
      bool swap_rows(int lhs, int rhs);
      //обмен столбцов
      bool swap_columns(int lhs, int rhs);

      T BareissDeterminant() const;

      public:
      // Мы бы хотели использовать m[x][y]
      ProxyRow operator[](int);
      const ProxyRow operator[](int) const;
    };

  template <class T>
    // конструктор для создания матрицы, заполненной значением
    Matrix<T>::Matrix(int cols, int rows, T val): cols_(cols), rows_(rows),
    data_(create_data(cols, rows, val)){
#ifdef my_debug
      std::cerr << "Matrix::" << __func__ <<" cols,rows,val \n";
#endif
    }
  // конструктор для создания из заданной последовательности
  template <class T> template< typename It>
    Matrix<T>::Matrix(int cols, int rows, It start, It fin):Matrix(cols, rows){
#ifdef my_debug
      std::cerr << "Matrix::" << __func__ <<" iterators start fin \n";
#endif
      assert(cols * rows == std::distance(start, fin) );
      if (cols * rows != std::distance(start, fin) ){
        throw std::invalid_argument("Iterators distamce dot equal Matrix capacity.");
      }
      for (int i = 0; i < rows ; ++i){
        for (int j = 0; j < cols ; ++j){
          data_[i][j] = *start;
          ++start;
        }
      }
    }

  // копирующий конструктор
  template <class T>
    Matrix<T>::Matrix(const Matrix &rhs)
    :cols_(rhs.ncols()), rows_(rhs.nrows()), 
    data_(copy_data(rhs.ncols(), rhs.nrows(), rhs.data_)){
#ifdef my_debug
      std::cerr << "Matrix::" << __func__ <<" copy ctor \n";
#endif
    }
  // перемещающий конструктор
  template <class T>
    Matrix<T>::Matrix(Matrix &&rhs) noexcept :cols_(rhs.ncols()), rows_(rhs.nrows()), data_(rhs.data_) {
      rhs.data_ = nullptr;
      rhs.cols_ = {};
      rhs.rows_ = {};
#ifdef my_debug
      std::cerr << "Matrix::" << __func__ <<" move ctor \n";
#endif
    }

  // присваивание 
  template <class T>
  typename Matrix<T>::Matrix& Matrix<T>::operator=(const Matrix &rhs){
#ifdef my_debug
      std::cerr << "Matrix::" << __func__ <<" copy \n";
#endif
    if (&rhs == this){
      return *this;
    }
    T** temp = copy_data(rhs.ncols(), rhs.nrows(), rhs.data_);
    std::swap(data_, temp);
    free_data(cols_, rows_, temp);
    cols_ = rhs.ncols(); 
    rows_ = rhs.nrows();

    return *this;
  }
 
  // перемещение
  template <class T>
  typename Matrix<T>::Matrix& Matrix<T>::operator=(Matrix &&rhs) noexcept{
#ifdef my_debug
      std::cerr << "Matrix::" << __func__ <<" move \n";
#endif
    if (&rhs == this){
      return *this;
    }
    std::swap(data_, rhs.data_);
    std::swap(cols_, rhs.cols_);
    std::swap(rows_, rhs.rows_);

    return *this;
  }
 
 
 
  // деструктор матрицы
  template <class T>
    Matrix<T>::~Matrix(){
#ifdef my_debug
      std::cerr << "Matrix::" << __func__ <<" destructor \n";
#endif
      free_data(*this);
    }

  // заполнение строки значениями
  template <class T>
    void Matrix<T>::fill_row(T *row, int cols, T val){
      if (nullptr == row){
        return;
      } 
      for (int i = 0 ; i < cols ; ++i){
        row[i] = val;
      }
    }

  // создание представления
  template <class T>
    T**  Matrix<T>::create_data(int cols, int rows, T val){
      T** data_ = new T*[rows];

      for (int i = 0 ; i < rows ; ++i){
        data_[i] = new T[cols];
        fill_row(data_[i], cols , val);
      }
      return data_;
    }

  // копирование представления
  template <class T>
    T**  Matrix<T>::copy_data(int cols, int rows, T** src){
      if (nullptr == src){
        return  src;
      }

      T** data_ = new T*[rows];

      for (int i = 0 ; i < rows ; ++i){
        data_[i] = new T[cols];
        std::copy(src[i], src[i] + cols, data_[i]);
      }
      return data_;
    }

  // удаление представления
  template <class T>
    void Matrix<T>::free_data(Matrix& m){
      free_data(m.ncols(), m.nrows(), m.data_);
      m.data_ = nullptr;
    }

  template <class T>
    void Matrix<T>::free_data(int , int rows, T** data){
      if (nullptr == data){
        return;
      } 
      for (int i = 0 , end = rows ; i < end ; ++i){
        delete[] data[i];
      }
      delete[] data;
    }

  template <class T>
    void Matrix<T>::dump(std::ostream& os) const{
      os << ncols() << ' ' << nrows() << '\n';
      if (nullptr == data_){
        os << "Empty matrix\n";
        return;
      }

      for(int i = 0 ; i < nrows() ; ++i){
        if (data_[i] == nullptr){
          os << "Empty matrix row\n";
          continue;
        }
        for(int j = 0 ; j < ncols() ; ++j){
          os.width(5);
          os << data_[i][j] << ' ' ;
        }
        os << '\n';
      }  

    }
  template <class T>
    int Matrix<T>::ncols() const {
      return cols_;
    }
  template <class T>
    int Matrix<T>::nrows() const {
      return rows_;
    }
  // след матрицы - сумма главной диагонали
  template <class T>
    T Matrix<T>::trace() const{
      T sum = T{};
      assert(ncols() == nrows());
      for (int i = 0, end = nrows(); i < end ; ++i){
        sum += data_[i][i];
      }
      return sum;
    }

  // равенство
  template <class T>
  bool Matrix<T>::equal(const Matrix& other) const{
    if (ncols() != other.ncols() || nrows() != other.nrows()){
      return false;
    }
    for (int i = 0 , end_i = nrows() ; i < end_i ; ++i){
      for (int j = 0 , end_j = ncols() ; j < end_j ; ++j){
        if (data_[i][j] != other.data_[i][j]){
          return false;
        }
      }
    }
    return true;
  }

  // меньше чем other
  template <class T>
  bool Matrix<T>::less(const Matrix& other) const{
    if (ncols() != other.ncols() || nrows() != other.nrows()){
      return false;
    }
    for (int i = 0 , end_i = nrows() ; i < end_i ; ++i){
      for (int j = 0 , end_j = ncols() ; j < end_j ; ++j){
        if ( ! (data_[i][j] < other.data_[i][j]) ){
          return false;
        }
      }
    }
    return true;
  }

  // отрицание
  template <class T>
    Matrix<T>&  Matrix<T>::negate() &{
      for (int i = 0 , end_i = nrows() ; i < end_i ; ++i){
        for (int j = 0 , end_j = ncols() ; j < end_j ; ++j){
          data_[i][j] = -data_[i][j];
        }
      }
      return *this;
    }
  
  // почему не Matrix transpose() const?
  template <class T>
    Matrix<T>&  Matrix<T>::transpose() &{
      Matrix<T> m(nrows(), ncols());
      for (int i = 0 , end_i = nrows() ; i < end_i ; ++i){
        for (int j = 0 , end_j = ncols() ; j < end_j ; ++j){
          m.data_[j][i] = data_[i][j];
        }
      }
      *this = std::move(m);
      return *this;
    }

  // "конструктор" для создания единичной матрицы
  template <class T>
    Matrix<T> Matrix<T>::eye(int cols, int rows, T val){
      Matrix<T> result{cols, rows, 0};
      assert(cols == rows);
      for (int i = 0; i < rows ; ++i){
        result.data_[i][i] = val;
      }

      return result;
    }
  // "конструктор" для создания случайной матрицы
  template <class T>
    Matrix<T> Matrix<T>::random(int cols, T determinant, T low, T high){
      Matrix<T> result = eye(cols, cols, T{1});
      result[0][0] = determinant;
      std::random_device rd;  // a seed source for the random number engine
      std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
      std::uniform_int_distribution<> distrib(low , high);

      // Use distrib to transform the random unsigned int
      // generated by gen into an int in [-6, 6]

      for (int i = 0; i < cols ; ++i){
        for (int j = i + 1; j < cols ; ++j){
          result.data_[i][j] = distrib(gen);
        }
      }
      // multiply on random number the first line and add to each line
      for (int i = 1; i < cols ; ++i){
        int multiply = distrib(gen);
        for (int j = 0 ; j < cols ; ++j){
          result.data_[i][j] = result.data_[i][j] +  result.data_[0][j] * multiply;
        }
      }
      if (cols > 1){
        // mix first line
        for (int j = 0 ; j < cols ; ++j){
          result.data_[0][j] = result.data_[0][j] +  result.data_[1][j];
        }
      }
      return result;
    }
  // Мы бы хотели использовать m[x][y]
  template <class T>
    typename  Matrix<T>::ProxyRow Matrix<T>::operator[](int row){
      return {data_[row]};
    }

  // Мы бы хотели использовать m[x][y]
  template <class T>
    const typename Matrix<T>::ProxyRow Matrix<T>::operator[](int row) const{
      return {data_[row]};
    }

  // обмен строк
  template <class T>
    bool Matrix<T>::swap_rows(int lhs, int rhs){
      std::swap(data_[lhs], data_[rhs]);
      return true;
    }

  //обмен столбцов
  template <class T>
    bool Matrix<T>::swap_columns(int lhs, int rhs){
      for(int i = 0, end = nrows(); i < end; ++i){
        std::swap(data_[i][lhs], data_[i][rhs]);
      }
      return true;
    }

  // https://web.archive.org/web/20100415172454/http://iproc.ru/parallel-programming/lection-4/
  template<class T> 
    T Matrix<T>::BareissDeterminant() const
    {

      Matrix<T> m(*this);
      T denom(1); //!
      int exchanges = 0;
      assert( ncols() == nrows() );

      for(int l1 = 0, l1_end = m.nrows() - 1 ; l1 < l1_end ; ++l1)
      { //Перебираю все строки матрицы, кроме последней
        int maxN = l1;
        T maxValue( m_abs( m[l1][l1] ) );
        for(int l2 = l1+1, l2_end = m.nrows(); l2 < l2_end ; ++l2)
        { //Нахожу строку с максимальным по модулю элементом
          T const value( m_abs( m[l2][l1] ) );
          if( value > maxValue ) {
            maxN = l2; maxValue = value; 
          }
        }

#ifdef my_debug_1
        std::cerr << "Matrix::" << __func__ <<" maxN:" << maxN <<" \n";
#endif
        if( maxN > l1 )
        { //Нужен обмен
          m.swap_rows(l1, maxN);
          ++exchanges;
#ifdef my_debug_1
          std::cerr << "Matrix::" << __func__ <<" exchanged \n";
#endif
        } else { //Обмен не нужен, но нужна проверка на ноль
          if( maxValue == T(0) ) {
            return maxValue;
          }
        }
        T const value1( m[l1][l1] ); //!
                                     //!

        for(int l2 = l1 + 1, l2_end = m.nrows(); l2 < l2_end ; ++l2)
        { //Вычитаю строку из всех последующих
          T const value2( m[l2][l1] ); //!
          m[l2][l1] = T(0);
          for(int c = l1+1 , c_end = m.nrows() ; c < c_end ; ++c) {//!
            m[l2][c] = ( m[l2][c] * value1- m[l1][c] * value2 ) / denom;
          }
        }

        denom = value1; //!
      }
      //!
      int last_index = m.nrows() - 1;
      T answer = m[last_index][last_index];
      if(exchanges & 1) {
        return -answer;
      } else {
        return answer;
      }
    }
}// namespace mm
