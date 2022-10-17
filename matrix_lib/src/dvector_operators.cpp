#include <iostream>

#include "stdafx.h"
#include "dvector.h"
#include "dmatrix.h"

extern const std::string ERROR_SIZE;

extern void IsEqualSize(size_t size1, size_t size2, std::string const &msgError);

// ---------------------------------------------------
// vector += vector

DVector &operator+=(DVector &left, DVector const &right) 
{
    IsEqualSize(left.Size(), right.Size(), "operator+= :" + ERROR_SIZE);
    for (size_t i = 0; i < left.Size(); ++i)
    {
        left[i] += right[i];
    }
    return left;
}

DVector &operator-=(DVector &left, DVector const &right) 
{
    IsEqualSize(left.Size(), right.Size(), "operator-= :" + ERROR_SIZE);
    for (size_t i = 0; i < left.Size(); ++i)
    {
        left[i] -= right[i];
    }
    return left;
}

DVector &operator*=(DVector &left, DVector const &right) 
{
    IsEqualSize(left.Size(), right.Size(), "operator*= :" + ERROR_SIZE);
    for (size_t i = 0; i < left.Size(); ++i)
    {
        left[i] *= right[i];
    }
    return left;
}

DVector &operator/=(DVector &left, DVector const &right) 
{
    IsEqualSize(left.Size(), right.Size(), "operator/= :" + ERROR_SIZE);
    for (size_t i = 0; i < left.Size(); ++i)
    {
        left[i] /= right[i];
    }
    return left;
}

// ---------------------------------------------------
// vector + vector

DVector operator+(DVector left, DVector const &right)
{
    IsEqualSize(left.Size(), right.Size(), "operator+ :" + ERROR_SIZE);
    return left += right;
}

DVector operator-(DVector left, DVector const &right)
{
    IsEqualSize(left.Size(), right.Size(), "operator- :" + ERROR_SIZE);
    return left -= right;
}

DVector operator*(DVector left, DVector const &right)
{
    IsEqualSize(left.Size(), right.Size(), "operator* :" + ERROR_SIZE);
    return left *= right;
}

DVector operator/(DVector left, DVector const &right)
{
    IsEqualSize(left.Size(), right.Size(), "operator/ :" + ERROR_SIZE);
    return left /= right;
}


// ---------------------------------------------------
// vector *= value

DVector &operator*=(DVector &left, double value) 
{
    for (size_t i = 0; i < left.Size(); ++i)
    {
        left[i] *= value;
    }
    return left;
}

DVector &operator/=(DVector &left, double value) 
{
    for (size_t i = 0; i < left.Size(); ++i)
    {
        left[i] /= value;
    }
    return left;
}

// ---------------------------------------------------
// vector * value

DVector operator*(DVector left, double value)
{
    return left *= value;
}

DVector operator/(DVector left, double value)
{
    return left /= value;
}
