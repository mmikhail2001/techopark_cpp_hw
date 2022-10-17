#pragma once

#include "dvector.h"

enum class  ORIENT  { ROW = 0, COL };
enum        SLICE   { ROW = 0, COL };

class DMatrix
{
private:
    DVector *m_matrix     = nullptr;
    size_t   m_nRows      = 0;
    size_t   m_nCols      = 0;
    size_t   m_capacity   = 0;
    void grow();
public:
    DMatrix() = default;
    DMatrix(DVector *matrix, size_t nRows);
    // Создание матрицы как вектор-строку или вектор-столбец
    DMatrix(DVector const &dvec, ORIENT orientation = ORIENT::ROW);
    DMatrix(size_t rows, size_t cols, double fill_value = 0);
    DMatrix(std::initializer_list<std::initializer_list<double>> const &matrix);
    DMatrix(DMatrix const &other);
    DMatrix(DMatrix &&other);
    DMatrix &operator=(DMatrix other);
    ~DMatrix();

    template <size_t rows = 0, size_t cols = 0> 
    static DMatrix Create(double fill_value = 0)
    {
        return DMatrix(rows, cols, fill_value);
    }

public:
    void    Clear();
    bool    Empty();
    void    Swap(DMatrix &other);

    void    PushRowBack(DVector const &dvec);
    void    PushRowBack(std::initializer_list<double> const &init_list);
    void    PopRowBack();

    void    PushColBack(DVector const &dvec);
    void    PushColBack(std::initializer_list<double> const &init_list);
    void    PopColBack();

    void    EraseByIndex(size_t index, ORIENT orientation = ORIENT::ROW);


    /* ****** ЗАДАНИЕ 1 ****** */ 
    DVector GetDiag() const;
    DVector GetRow(size_t index) const;
    DVector GetCol(size_t index) const;

    /* ****** ЗАДАНИЕ 4 ****** */ 
    DMatrix Dot(DMatrix const &other) const;
    DVector Dot(DVector const &dvec) const;

    size_t  nRows() const;
    size_t  nCols() const;
    size_t  Capacity() const;

    /* ****** ЗАДАНИЕ 5 ****** */
    // модифицирующие операции
    void    AddNum(double value);
    void    SubNum(double value);
    void    AddVec(DVector const &dvec, ORIENT orientation = ORIENT::ROW);
    void    SubVec(DVector const &dvec, ORIENT orientation = ORIENT::ROW);

    /* ****** ЗАДАНИЯ 6 и 7 ****** */
    DMatrix T() const;
    double  Det() const;
    double  Minor(size_t iIndex, size_t jIndex) const;
    DMatrix Adj();
    DMatrix Inv();
    
    DVector const    &operator[](size_t index) const;
    DVector          &operator[](size_t index);

    /*
        Slicing как в NumPy C++
        matrix({1, 4}, {2, 5}) - нельзя отдельно по столбцам
    
        enum SLICE {ROW = 0, COL}, чтобы 
            1. не писать класс, если в параметрах ROW / COL
            2. кастилось к enum SLICE, если в параметрах 0 / 1
    */

   /* ****** Slices ****** */
    DMatrix operator()(size_t begin, size_t end, int step = 1, uint8_t sliceType = SLICE::ROW) const;

    DMatrix SliceRow(size_t begin, size_t end, int step = 1) const;
    DMatrix SliceCol(size_t begin, size_t end, int step = 1) const;
};

// matrix *= value
DMatrix &operator/=(DMatrix &matrix, double value);
DMatrix &operator*=(DMatrix &matrix, double value);

// matrix * value
DMatrix  operator/(DMatrix matrix, double value);
DMatrix  operator*(DMatrix matrix, double value);

// value *= matrix

DMatrix &operator/=(double value, DMatrix &matrix);
DMatrix &operator*=(double value, DMatrix &matrix);

// value * matrix
DMatrix  operator/(double value, DMatrix matrix);
DMatrix  operator*(double value, DMatrix matrix);

// matrix += matrix

DMatrix &operator+=(DMatrix &left, DMatrix const &right);
DMatrix &operator-=(DMatrix &left, DMatrix const &right);
DMatrix &operator/=(DMatrix &left, DMatrix const &right);
DMatrix &operator*=(DMatrix &left, DMatrix const &right);

// matrix + matrix
DMatrix  operator+(DMatrix left, DMatrix const &right);
DMatrix  operator-(DMatrix left, DMatrix const &right);
DMatrix  operator/(DMatrix left, DMatrix const &right);
DMatrix  operator*(DMatrix left, DMatrix const &right);

void Print(DMatrix const &matrix, std::string const &msg = std::string{});