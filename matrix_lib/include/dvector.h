#pragma once

#include "stdafx.h"

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
    void Swap(DVector &other);
    DVector &operator=(DVector other);
    const double *CBegin();
    const double *CEnd();
    bool Empty() const;
    size_t Size() const;
    void Clear();
    size_t Capacity() const;
    void PushBack(double value);
    double *Find(double value) const;
    double *Erase(double *ptr_value);
    void PopBack();
    double Front() const;
    double Back() const;
    double const &operator[](size_t index) const;
    double &operator[](size_t index);
    ~DVector();
};

void Print(DVector const &dvector, std::string const &message = std::string{});