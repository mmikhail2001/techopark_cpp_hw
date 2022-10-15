#include <iostream>

#include "stdafx.h"
#include "dvector.h"

DVector::DVector(size_t size, double fill_value) : m_size(size), m_capacity(size)
{
    m_array = new double[m_capacity]();
    if (fill_value)
    {
        std::fill_n(m_array, m_size, fill_value);
    }
}

DVector::DVector(std::initializer_list<double> const &init_list) : m_size(init_list.size()), m_capacity(init_list.size())
{
    m_array = new double[init_list.size()]();
    std::copy(init_list.begin(), init_list.end(), m_array);
}

DVector::DVector(DVector const &other)
{
    if (m_array)
    {
        delete[] m_array;
    }
    m_array = new double[other.m_capacity]();
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    std::copy(other.m_array, &other.m_array[other.m_size - 1] + 1, m_array);
}

DVector::DVector(DVector &&other) : m_array(nullptr), m_capacity(0), m_size(0)
{
    Swap(other);
}

DVector::DVector(const double *begin, const double *end)
{
    ptrdiff_t length = std::distance(begin, end);
    m_array = new double[length]();
    m_size = m_capacity = length;
    std::copy(begin, end, m_array);
}

DVector &DVector::operator=(DVector other)
{
    Swap(other);
    return *this;
}

const double *DVector::CBegin()
{
    return m_array;
}

// указатель на элемент после последнего
const double *DVector::CEnd()
{
    return &m_array[m_size - 1] + 1;
}

void DVector::Swap(DVector &other)
{
    std::swap(m_array, other.m_array);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_size, other.m_size);
}

double const &DVector::operator[](size_t index) const
{
    if (index > m_size - 1)
    {
        throw std::runtime_error("Index out of range");
    }
    return m_array[index];
}

double &DVector::operator[](size_t index)
{
    if (index > m_size - 1)
    {
        throw std::runtime_error("Index out of range");
    }
    return m_array[index];
}

void DVector::PushBack(double value)
{
    if (m_size == m_capacity)
    {
        grow();
    }
    m_array[m_size++] = value;
}

bool DVector::Empty() const
{
    return m_size == 0;
}

void DVector::PopBack()
{
    if (Empty())
    {
        throw std::runtime_error("PopBack() from empty vector");
    }
    --m_size;
}

size_t DVector::Size() const
{
    return m_size;
}

size_t DVector::Capacity() const
{
    return m_capacity;
}

void DVector::Clear() 
{
    if (m_array)
    {
        delete[] m_array;
    }
    m_array = nullptr;
    m_size = 0;
    m_capacity = 0;
}

double DVector::Front() const
{
    if (Empty())
    {
        throw std::runtime_error("From() from empty vector");
    }
    return m_array[0];
}

double DVector::Back() const
{
    if (Empty())
    {
        throw std::runtime_error("Back() from empty vector");
    }
    return m_array[m_size - 1];
}

DVector::~DVector()
{
    if (m_array)
    {
        delete[] m_array;
    }
}

double *DVector::Find(double value) const
{
    double *it_end = &m_array[m_size - 1] + 1;
    double *result = std::find(m_array, it_end, value);
    if (result == it_end)
    {
        return nullptr;
    }
    return result; 
}

// возвращает элемент, следующий за удаленным либо предыдущий, если удаленный был последним
double *DVector::Erase(double *it_value)
{
    ptrdiff_t diff = reinterpret_cast<uintptr_t>(it_value) -  reinterpret_cast<uintptr_t>(m_array);
    if (!it_value || it_value < m_array || it_value > &m_array[m_size - 1] || diff % sizeof(double) != 0)
    {
        return nullptr;
    }
    for (double *it = it_value; it < &m_array[m_size - 1]; it++)
    {
        *it = *(it + 1); 
    }
    --m_size;
    return it_value == &m_array[m_size - 1] + 1 ? it_value - 1 : it_value;

}

void DVector::grow()
{
    size_t new_capacity = std::max(1, static_cast<int>(m_capacity * 2));
    double *new_array = new double[new_capacity]();
    std::copy(m_array, &m_array[m_size - 1] + 1, new_array);
    delete[] m_array;
    m_array = new_array;
    m_capacity = new_capacity;
}

void Print(DVector const &dvector, std::string const &message)
{
    if (!message.empty())
    {
        std::cout << message << std::endl;
    }
    for (size_t i = 0; i < dvector.Size(); ++i)
    {
        std::cout << dvector[i] << " ";
    }
    std::cout << std::endl;
}
