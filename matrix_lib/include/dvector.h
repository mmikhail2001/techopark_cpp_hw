#pragma once

#include "stdafx.h"

class DMatrix;

// Vector of doubles number
class DVector
{
private:
    double *m_array     = nullptr;
    size_t m_capacity   = 0;
    size_t m_size       = 0;
    void grow();
public:
    DVector() = default;
    explicit DVector(size_t size, double fill_value = 0);
    DVector(const double *begin, const double *end);
    DVector(std::initializer_list<double> const &init_list);
    DVector(DVector const &other);
    DVector(DVector &&other);
    DVector &operator=(DVector other);
    ~DVector();
    
public:
    const double *CBegin() const;
    const double *CEnd() const;

    void    Swap(DVector &other);
    void    Fill(double fill_value);

    bool    Empty() const;
    size_t  Size() const;
    size_t  Capacity() const;
    void    Clear();
    
    double *Find(double value) const;
    double *Erase(double *ptr_value);
    
    void    PushBack(double value);
    void    PopBack();
    
    double  Front() const;
    double  Back() const;

    double const    &operator[](size_t index) const;
    double          &operator[](size_t index);

    /* ****** ЗАДАНИЕ 4 ****** */
    double  Dot(DVector const &other) const;
    DVector Dot(DMatrix const &matrix) const;


    /* ****** ЗАДАНИЕ 5 ****** */
    // модифицирующие операции
    void AddNum(double value);
    void SubNum(double value);

    /* ****** Slices ****** */
    DVector operator()(size_t begin, size_t end, int step = 1) const;
};

// vector *= value
DVector &operator/=(DVector &left, double value);
DVector &operator*=(DVector &left, double value);

// vector * value
DVector  operator/(DVector left, double value);
DVector  operator*(DVector left, double value);

// vector += vector
DVector &operator+=(DVector &left, DVector const &right);
DVector &operator-=(DVector &left, DVector const &right);
DVector &operator/=(DVector &left, DVector const &right);
DVector &operator*=(DVector &left, DVector const &right);

// vector + vector
DVector  operator+(DVector left, DVector const &right);
DVector  operator-(DVector left, DVector const &right);
DVector  operator/(DVector left, DVector const &right);
DVector  operator*(DVector left, DVector const &right);

void Print(DVector const &dvector, std::string const &message = std::string{});