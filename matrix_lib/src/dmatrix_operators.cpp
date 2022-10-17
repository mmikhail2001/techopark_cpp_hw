#include "stdafx.h"
#include "dmatrix.h"

extern const std::string ERROR_EMPTY;
extern const std::string ERROR_SIZE;

extern void IsEqualSize(size_t size1, size_t size2, std::string const &msgError);

// ---------------------------------------------------
// value *= matrix

DMatrix &operator*=(double value, DMatrix &matrix) 
{
    return matrix *= value;
}

DMatrix &operator/=(double value, DMatrix &matrix) 
{
    return matrix /= value;
}

// ---------------------------------------------------
// value * matrix

DMatrix operator*(double value, DMatrix matrix)
{
    return matrix *= value;
}

DMatrix operator/(double value, DMatrix matrix)
{
    return matrix /= value;
}

// ---------------------------------------------------
// matrix += matrix

DMatrix &operator+=(DMatrix &left, DMatrix const &right) 
{
    IsEqualSize(left.nRows(), right.nRows(), "operator+= :" + ERROR_SIZE);
    IsEqualSize(left.nCols(), right.nCols(), "operator+= :" + ERROR_SIZE);
    for (size_t i = 0; i < left.nRows(); ++i)
    {
        left[i] += right[i];
    }
    return left;
}

DMatrix &operator-=(DMatrix &left, DMatrix const &right) 
{
    IsEqualSize(left.nRows(), right.nRows(), "operator-= :" + ERROR_SIZE);
    IsEqualSize(left.nCols(), right.nCols(), "operator-= :" + ERROR_SIZE);
    for (size_t i = 0; i < left.nRows(); ++i)
    {
        left[i] -= right[i];
    }
    return left;
}

DMatrix &operator*=(DMatrix &left, DMatrix const &right) 
{
    IsEqualSize(left.nRows(), right.nRows(), "operator*= :" + ERROR_SIZE);
    IsEqualSize(left.nCols(), right.nCols(), "operator*= :" + ERROR_SIZE);
    for (size_t i = 0; i < left.nRows(); ++i)
    {
        left[i] *= right[i];
    }
    return left;
}

DMatrix &operator/=(DMatrix &left, DMatrix const &right) 
{
    IsEqualSize(left.nRows(), right.nRows(), "operator/= :" + ERROR_SIZE);
    IsEqualSize(left.nCols(), right.nCols(), "operator/= :" + ERROR_SIZE);
    for (size_t i = 0; i < left.nRows(); ++i)
    {
        left[i] /= right[i];
    }
    return left;
}

// ---------------------------------------------------
// matrix + matrix

DMatrix operator+(DMatrix left, DMatrix const &right)
{
    IsEqualSize(left.nRows(), right.nRows(), "operator+ :" + ERROR_SIZE);
    IsEqualSize(left.nCols(), right.nCols(), "operator+ :" + ERROR_SIZE);
    return left += right;
}

DMatrix operator-(DMatrix left, DMatrix const &right)
{
    IsEqualSize(left.nRows(), right.nRows(), "operator- :" + ERROR_SIZE);
    IsEqualSize(left.nCols(), right.nCols(), "operator-:" + ERROR_SIZE);
    return left -= right;
}

DMatrix operator*(DMatrix left, DMatrix const &right)
{
    IsEqualSize(left.nRows(), right.nRows(), "operator* :" + ERROR_SIZE);
    IsEqualSize(left.nCols(), right.nCols(), "operator* :" + ERROR_SIZE);
    return left *= right;
}

DMatrix operator/(DMatrix left, DMatrix const &right)
{
    IsEqualSize(left.nRows(), right.nRows(), "operator/ :" + ERROR_SIZE);
    IsEqualSize(left.nCols(), right.nCols(), "operator/ :" + ERROR_SIZE);
    return left /= right;
}

// ---------------------------------------------------
// matrix *= value

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

// ---------------------------------------------------
// matrix * value

DMatrix operator*(DMatrix matrix, double value)
{
    return matrix *= value;
}

DMatrix operator/(DMatrix matrix, double value)
{
    return matrix /= value;
}

// --------------------------------------------------------