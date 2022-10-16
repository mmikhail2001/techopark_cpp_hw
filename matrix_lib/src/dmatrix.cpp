#include <dmatrix.h>

#include "stdafx.h"
#include "dmatrix.h"

const std::string ERROR_EMPTY = "DMatrix is empty";

DMatrix::DMatrix(size_t nRows, size_t nCols, double fill_value) : m_nRows(nRows), m_nCols(nCols), m_capacity(nRows)
{
    // можно ли указать конкретный конструктор для каждого элемента дин. массива?
    m_matrix = new DVector[m_nRows]();
    for (size_t i = 0; i < m_nRows; ++i)
    {
        m_matrix[i] = DVector(m_nCols, fill_value);
    }
}

DMatrix::DMatrix(std::initializer_list<std::initializer_list<double>> const &init_list) : m_nRows(init_list.size()), m_nCols(init_list.begin()->size()), m_capacity(init_list.size())
{
    if (std::any_of(init_list.begin() + 1, init_list.end(), [n = m_nCols](std::initializer_list<double> const &list)
    {
        return list.size() != n;
    }))
    {
        throw std::runtime_error("The matrix has rows of different lengths");
    }
    m_matrix = new DVector[m_nRows]();
    int i = 0;
    for (auto row = init_list.begin(); row < init_list.end(); ++row)
    {
        m_matrix[i] = DVector(row->begin(), row->end());
        ++i;
    }
}

DMatrix::DMatrix(DMatrix const &other) : m_nRows(other.nRows()), m_nCols(other.nCols()), m_capacity(other.nRows())
{
    m_matrix = new DVector[other.nRows()]();
    for (size_t i = 0; i < other.m_nRows; ++i)
    {
        m_matrix[i] = DVector(other[i].CBegin(), other[i].CEnd());
    }
}

DMatrix::DMatrix(DMatrix &&other) : m_matrix(other.m_matrix), m_nRows(other.m_nRows), m_nCols(m_nCols), m_capacity(other.m_nRows)
{
    other.Clear();
}

DMatrix::DMatrix(DVector *matrix, size_t nRows) : m_matrix(matrix), m_nRows(nRows), m_nCols(matrix[0].Size()), m_capacity(nRows)
{
    // намного читабильнее, чем std::any_of....
    for (size_t i = 1; i < m_nCols; ++i)
    {
        if (matrix[i].Size() != m_nCols)
        {   
            throw std::runtime_error("The matrix has rows of different lengths");
        }
    }
}

DMatrix::~DMatrix()
{
    if (m_matrix)
    {
        delete[] m_matrix;
    }
    m_matrix = nullptr;
}

void DMatrix::Swap(DMatrix &other)
{
    std::swap(m_matrix, other.m_matrix);
    std::swap(m_nRows, other.m_nRows);
    std::swap(m_nCols, other.m_nCols);
}

DMatrix &DMatrix::operator=(DMatrix other)
{
    Swap(other);
    return *this;
}

DMatrix::DMatrix(DVector const &dvec) : m_nRows(1), m_nCols(dvec.Size())
{
    m_matrix = new DVector[1]{ DVector(dvec) };
}

void DMatrix::Clear()
{
    m_matrix = nullptr;
    m_nRows = 0;
    m_nCols = 0;
    m_capacity = 0;
}

size_t DMatrix::nRows() const
{
    return m_nRows;
}

size_t DMatrix::nCols() const
{
    return m_nCols;
}


DVector const &DMatrix::operator[](size_t index) const
{
    if (index > m_nRows - 1)
    {
        throw std::runtime_error("Index out of range");
    }
    return m_matrix[index];
}

DVector &DMatrix::operator[](size_t index) 
{
    if (index > m_nRows - 1)
    {
        throw std::runtime_error("Index out of range");
    }
    return m_matrix[index];
}

bool DMatrix::Empty()
{
    return m_nRows == 0;
}

size_t DMatrix::Capacity() const
{
    return m_capacity;
}

void DMatrix::PushRowBack(DVector const &dvec)
{
    if (m_nRows > 0 && dvec.Size() != m_nCols)
    {
        throw std::runtime_error("The matrix has rows of different lengths");
    }
    m_nCols = dvec.Size();
    if (m_nRows == m_capacity)
    {
        grow();
    }
    m_matrix[m_nRows++] = dvec;
}

void DMatrix::PushRowBack(std::initializer_list<double> const &init_list)
{
    if (m_nRows > 0 && init_list.size() != m_nCols)
    {
        throw std::runtime_error("The matrix has rows of different lengths");
    }
    m_nCols = init_list.size();
    if (m_nRows == m_capacity)
    {
        grow();
    }
    m_matrix[m_nRows++] = DVector(init_list.begin(), init_list.end());
}

void DMatrix::PopRowBack()
{
    if (Empty())
    {
        throw std::runtime_error("PopBack(): " + ERROR_EMPTY);
    }
    m_matrix[--m_nRows].~DVector();
    if (m_nRows == 0)
    {
        if (m_matrix)
        {
            delete[] m_matrix;
        }
        Clear();
    }
}

void DMatrix::PushColBack(DVector const &dvec)
{
    if (m_nRows > 0 && dvec.Size() != m_nRows)
    {
        throw std::runtime_error("The matrix has rows of different lengths");
    }
    if (m_nRows == 0)
    {
        m_matrix = new DVector[dvec.Size()]();
        m_capacity = dvec.Size();
    }
    m_nRows = dvec.Size();
    ++m_nCols;
    for (size_t i = 0; i < m_nRows; ++i)
    {
        m_matrix[i].PushBack(dvec[i]);
    }
}

void DMatrix::PushColBack(std::initializer_list<double> const &init_list)
{
    if (m_nRows > 0 && init_list.size() != m_nRows)
    {
        throw std::runtime_error("The matrix has rows of different lengths");
    }
    if (m_nRows == 0)
    {
        m_matrix = new DVector[init_list.size()]();
        m_capacity = init_list.size();
    }
    m_nRows = init_list.size();
    ++m_nCols;
    for (size_t i = 0; i < m_nRows; ++i)
    {
        m_matrix[i].PushBack(*(init_list.begin() + i));
    }
}

void DMatrix::PopColBack()
{
    if (Empty())
    {
        throw std::runtime_error("PopBack(): " + ERROR_EMPTY);
    }
    --m_nCols;
    for (size_t i = 0; i < m_nRows; ++i)
    {
        m_matrix[i].PopBack();
    }
    if (m_nCols == 0)
    {
        if (m_matrix)
        {
            delete[] m_matrix;
        }
        Clear();
    }
}

DVector DMatrix::GetDiag() const
{

}


void DMatrix::grow()
{
    size_t new_capacity = std::max(1, static_cast<int>(m_capacity * 2));
    DVector *new_matrix = new DVector[new_capacity]();
    std::copy(m_matrix, &m_matrix[m_nRows - 1] + 1, new_matrix);
    delete[] m_matrix;
    m_matrix = new_matrix;
    m_capacity = new_capacity;
}


void Print(DMatrix const &matrix, std::string const &msg)
{
    if (!msg.empty())
    {
        std::cout << msg << std::endl;
    }
    for (size_t i = 0; i < matrix.nRows(); ++i)
    {
        Print(matrix[i]);
    }
}