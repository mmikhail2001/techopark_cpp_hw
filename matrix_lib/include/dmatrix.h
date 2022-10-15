#pragma once

#include "dvector.h"

class DMatrix
{
private:
    DVector *m_matrix   = nullptr;
    size_t m_nRows      = 0;
    size_t m_nCols      = 0;
    size_t m_capacity   = 0;
    void grow();
public:
    DMatrix() = default;
    DMatrix(DVector *matrix, size_t nRows);
    DMatrix(DVector const &dvec);
    DMatrix(size_t rows, size_t cols, double fill_value = 0);
    DMatrix(std::initializer_list<std::initializer_list<double>> const &matrix);
    DMatrix(DMatrix const &other);
    void Clear();
    bool Empty();
    DMatrix(DMatrix &&other);
    
    void PushRowBack(DVector const &dvec);
    void PushRowBack(std::initializer_list<double> const &init_list);
    void PopRowBack();

    void PushColBack(DVector const &dvec);
    void PushColBack(std::initializer_list<double> const &init_list);
    void PopColBack();

    void Swap(DMatrix &other);
    DMatrix &operator=(DMatrix other);
    size_t nRows() const;
    size_t nCols() const;
    size_t Capacity() const;
    ~DMatrix();
    DVector const    &operator[](size_t index) const;
    DVector          &operator[](size_t index);
};

void Print(DMatrix const &matrix, std::string const &msg = std::string{});