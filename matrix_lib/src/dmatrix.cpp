#include <dmatrix.h>

#include "stdafx.h"
#include "dmatrix.h"

const std::string ERROR_EMPTY = "DMatrix is empty";
const std::string ERROR_ARITHMETIC = "Arithmetic operations on matrix of different size";

// проверка на равный размер, иначе исключение
static void IsEqualSize(size_t size1, size_t size2, std::string const &msgError = "Exception")
{
    if (size1 != size2)
    {
        throw std::runtime_error(msgError);
    }
}

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
    // проверка на прямоугольность init_list
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

DMatrix::DMatrix(DMatrix &&other) : m_matrix(other.m_matrix), m_nRows(other.m_nRows), m_nCols(other.nCols()), m_capacity(other.m_nRows)
{
    // обнуление
    other.Clear(); 
}

DMatrix::DMatrix(DVector *matrix, size_t nRows) : m_matrix(matrix), m_nRows(nRows), m_nCols(matrix[0].Size()), m_capacity(nRows)
{
    // проверка на прямоугольность DVector *
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

DMatrix::DMatrix(DVector const &dvec, ORIENT orientation)
{
    if (orientation == ORIENT::ROW)
    {
        m_matrix = new DVector[1]{ DVector(dvec) };
        m_nRows = 1;
        m_nCols = dvec.Size();
        m_capacity = m_nRows;
    }
    else
    {
        m_matrix = new DVector[dvec.Size()]();
        for (size_t i = 0; i < dvec.Size(); ++i)
        {
            m_matrix[i] = DVector(1, dvec[i]);
        }
        m_nRows = dvec.Size();
        m_nCols = 1;
        m_capacity = m_nRows;
    }
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
    size_t nElemsDiag = std::min(m_nRows, m_nCols); 
    DVector dvec(nElemsDiag);
    for (size_t i = 0; i < nElemsDiag; ++i)
    {
        dvec[i] = m_matrix[i][i];
    }
    return dvec;
}

DVector DMatrix::GetRow(size_t index) const
{
    DVector dvec(m_nCols);
    for (size_t i = 0; i < m_nCols; ++i)
    {
        dvec[i] = m_matrix[index][i];
    }
    return dvec;
}

DVector DMatrix::GetCol(size_t index) const
{
    DVector dvec(m_nRows);
    for (size_t i = 0; i < m_nRows; ++i)
    {
        dvec[i] = m_matrix[i][index];
    }
    return dvec;
}

DMatrix DMatrix::Dot(DMatrix const &other) const
{
    IsEqualSize(m_nCols, other.nRows(), ERROR_ARITHMETIC);

    DMatrix dmat(m_nRows, other.nCols());
    for (size_t i = 0; i < m_nRows; ++i)
    {
        for (size_t j = 0; j < other.nCols(); ++j)
        {
            double elem_i_j = 0;
            for (size_t k = 0; k < other.nRows(); ++k)
            {
                elem_i_j += m_matrix[i][k] * other.m_matrix[k][j];
            }
            dmat[i][j] = elem_i_j;
        }
    } 
    return dmat;
}

// подразуемевается что матрица умножается на вектор-столбец
// результат - DVector
DVector DMatrix::Dot(DVector const &dvec) const
{
    IsEqualSize(m_nCols, dvec.Size(), ERROR_ARITHMETIC);

    DVector dvecRes(m_nRows);
    for (size_t i = 0; i < m_nRows; ++i)
    {
        double elem_i_j = 0;
        for (size_t j = 0; j < m_nCols; ++j)
        {
            elem_i_j += m_matrix[i][j] * dvec[j];
        }
        dvecRes[i] = elem_i_j;
    }
    return dvecRes;
}

void DMatrix::AddNum(double value)
{
    std::for_each(m_matrix, &m_matrix[m_nRows - 1] + 1, [value](DVector &vec)
    {
        vec.AddNum(value);
    });
}

void DMatrix::SubNum(double value)
{
    std::for_each(m_matrix, &m_matrix[m_nRows - 1] + 1, [value](DVector &vec)
    {
        vec.SubNum(value);
    });
}

void DMatrix::AddVec(DVector const &dvec, ORIENT orientation)
{
    if (orientation == ORIENT::ROW)
    {
        IsEqualSize(dvec.Size(), m_nCols, ERROR_ARITHMETIC);
        // почему capture принимает по ссылке, хотя в сигнатуре const?
        std::for_each(m_matrix, &m_matrix[m_nRows - 1] + 1, [&dvec](DVector &vec)
        {
            vec += dvec;
        });
    }
    else
    {
        IsEqualSize(dvec.Size(), m_nRows, ERROR_ARITHMETIC);
        for (size_t i = 0; i < dvec.Size(); ++i)
        {
            m_matrix[i].AddNum(dvec[i]);
        }
    }
}

void DMatrix::SubVec(DVector const &dvec, ORIENT orientation)
{
    if (orientation == ORIENT::ROW)
    {
        IsEqualSize(dvec.Size(), m_nCols, ERROR_ARITHMETIC);
        std::for_each(m_matrix, &m_matrix[m_nRows - 1] + 1, [&dvec](DVector &vec)
        {
            vec -= dvec;
        });
    }
    else
    {
        IsEqualSize(dvec.Size(), m_nRows, ERROR_ARITHMETIC);
        for (size_t i = 0; i < dvec.Size(); ++i)
        {
            m_matrix[i].SubNum(dvec[i]);
        }
    }
}

DMatrix DMatrix::T() const
{
    IsEqualSize(m_nRows, m_nCols, "Matrix is not square");
    DMatrix dmatRes(m_nRows, m_nCols);
    for (size_t i = 0; i < m_nRows; ++i)
    {
        for (size_t j = 0; j < m_nCols; ++j)
        {
            dmatRes[j][i] = m_matrix[i][j];
        }
    }
    return dmatRes;
}

double DMatrix::Det() const
{
    IsEqualSize(m_nRows, m_nCols, "Matrix is not square");
    if (m_nRows == 2)
    {
        return m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0]; 
    }
    else
    {
        double res = 0;
        DMatrix *dmats = new DMatrix[m_nCols](); 
        // определитель матрицы через разложение по первой строке
        for (size_t i = 0; i < m_nCols; ++i)
        {
            dmats[i] = *this;
            dmats[i].EraseByIndex(0, ORIENT::ROW);
            dmats[i].EraseByIndex(i, ORIENT::COL);
            int8_t sign = i % 2 == 0 ? 1 : -1;
            res += sign * m_matrix[0][i] * dmats[i].Det();
        }
        delete[] dmats;
        return res;
    }
}

double DMatrix::Minor(size_t iIndex, size_t jIndex) const
{
    DMatrix dmat = *this;
    dmat.EraseByIndex(iIndex, ORIENT::ROW);
    dmat.EraseByIndex(jIndex, ORIENT::COL);
    return dmat.Det();
}

DMatrix DMatrix::Adj()
{
    DMatrix adjMat(m_nRows, m_nCols);
    for (size_t i = 0; i < m_nRows; ++i)
    {
        for (size_t j = 0; j < m_nCols; ++j)
        {
            int8_t sign = (i + j) % 2 == 0 ? 1 : -1;
            adjMat[i][j] = sign * Minor(i, j);
        }
    }
    return adjMat;
}

DMatrix DMatrix::Inv()
{
    double det = Det();
    if (det == 0)
    {
        return DMatrix{};
    }
    DMatrix invMat = (1 / det) * Adj().T();
    return invMat;
}

void DMatrix::EraseByIndex(size_t index, ORIENT orientation)
{
    if (m_nRows == 0)
    {
        throw std::runtime_error("Can't erase by index, Matrix is Empty");    
    }
    if (orientation == ORIENT::ROW)
    {
        if (index > m_nRows - 1)
        {
            throw std::runtime_error("Can't erase row by index");
        }
        for (size_t i = index; i < m_nRows - 1; ++i)
        {
            m_matrix[i] = m_matrix[i + 1];
        }
        m_matrix[--m_nRows].~DVector();
        if (m_nRows == 0)
        {
            Clear();
        }
    }
    else
    {
        if (index > m_nCols - 1)
        {
            throw std::runtime_error("Can't erase col by index");
        }
        for (size_t i = 0; i < m_nRows; ++i)
        {
            m_matrix[i].Erase(&m_matrix[i][index]);
        }    
        m_nCols--;
        if (m_nCols == 0)
        {
            Clear();
        }
    }
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

// -------------------------------------------------------------

DMatrix &operator*=(DMatrix &matrix, double value) 
{
    for (size_t i = 0; i < matrix.nRows(); ++i)
    {
        matrix[i] *= value;
    }
    return matrix;
}

DMatrix &operator/=(DMatrix &matrix, double value) 
{
    for (size_t i = 0; i < matrix.nRows(); ++i)
    {
        matrix[i] /= value;
    }
    return matrix;
}


DMatrix operator*(DMatrix matrix, double value)
{
    return matrix *= value;
}

DMatrix operator/(DMatrix matrix, double value)
{
    return matrix /= value;
}

// ---------- --------------- -------------

DMatrix &operator*=(double value, DMatrix &matrix) 
{
    return matrix *= value;
}

DMatrix &operator/=(double value, DMatrix &matrix) 
{
    return matrix /= value;
}


DMatrix operator*(double value, DMatrix matrix)
{
    return matrix *= value;
}

DMatrix operator/(double value, DMatrix matrix)
{
    return matrix /= value;
}

// -------------------------------------------------------------

DMatrix &operator+=(DMatrix &left, DMatrix const &right) 
{
    IsEqualSize(left.nRows(), right.nRows(), ERROR_ARITHMETIC);
    IsEqualSize(left.nCols(), right.nCols(), ERROR_ARITHMETIC);
    for (size_t i = 0; i < left.nRows(); ++i)
    {
        left[i] += right[i];
    }
    return left;
}

DMatrix &operator-=(DMatrix &left, DMatrix const &right) 
{
    IsEqualSize(left.nRows(), right.nRows(), ERROR_ARITHMETIC);
    IsEqualSize(left.nCols(), right.nCols(), ERROR_ARITHMETIC);
    for (size_t i = 0; i < left.nRows(); ++i)
    {
        left[i] -= right[i];
    }
    return left;
}

DMatrix &operator*=(DMatrix &left, DMatrix const &right) 
{
    IsEqualSize(left.nRows(), right.nRows(), ERROR_ARITHMETIC);
    IsEqualSize(left.nCols(), right.nCols(), ERROR_ARITHMETIC);
    for (size_t i = 0; i < left.nRows(); ++i)
    {
        left[i] *= right[i];
    }
    return left;
}

DMatrix &operator/=(DMatrix &left, DMatrix const &right) 
{
    IsEqualSize(left.nRows(), right.nRows(), ERROR_ARITHMETIC);
    IsEqualSize(left.nCols(), right.nCols(), ERROR_ARITHMETIC);
    for (size_t i = 0; i < left.nRows(); ++i)
    {
        left[i] /= right[i];
    }
    return left;
}

// --------------------------------------------------------

DMatrix operator+(DMatrix left, DMatrix const &right)
{
    IsEqualSize(left.nRows(), right.nRows(), ERROR_ARITHMETIC);
    IsEqualSize(left.nCols(), right.nCols(), ERROR_ARITHMETIC);
    return left += right;
}

DMatrix operator-(DMatrix left, DMatrix const &right)
{
    IsEqualSize(left.nRows(), right.nRows(), ERROR_ARITHMETIC);
    IsEqualSize(left.nCols(), right.nCols(), ERROR_ARITHMETIC);
    return left -= right;
}

DMatrix operator*(DMatrix left, DMatrix const &right)
{
    IsEqualSize(left.nRows(), right.nRows(), ERROR_ARITHMETIC);
    IsEqualSize(left.nCols(), right.nCols(), ERROR_ARITHMETIC);
    return left *= right;
}

DMatrix operator/(DMatrix left, DMatrix const &right)
{
    IsEqualSize(left.nRows(), right.nRows(), ERROR_ARITHMETIC);
    IsEqualSize(left.nCols(), right.nCols(), ERROR_ARITHMETIC);
    return left /= right;
}

// --------------------------------------------------------


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