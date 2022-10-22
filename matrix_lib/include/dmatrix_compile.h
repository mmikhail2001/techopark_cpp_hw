#include "dmatrix.h"
#include "dvector.h"

#define ERROR_LENGTH    "In a matrix, all row and column lengths must be the same"
#define ERROR_RANGE     "Index out of range"
#define ERROR_SIZE      "An arithmetic operation is not possible because of different sizes"

template <size_t rows, size_t cols>
class DMatrixCompile
{
private:
    DMatrix dmatRuntime;
public:
    DMatrixCompile(double fill_value = 0);

    size_t nRows() { return rows; }
    size_t nCols() { return cols; }

    DMatrix GetDynamicMatrix() const { return dmatRuntime; } 

    template <size_t rowsOther, size_t colsOther>
    DMatrix operator+(DMatrixCompile<rowsOther, colsOther>  const &dmatCompile);

    template <size_t rowsOther, size_t colsOther>
    DMatrix operator-(DMatrixCompile<rowsOther, colsOther>  const &dmatCompile);

    template <size_t rowsOther, size_t colsOther>
    DMatrix operator*(DMatrixCompile<rowsOther, colsOther>  const &dmatCompile);

    template <size_t rowsOther, size_t colsOther>
    DMatrix operator/(DMatrixCompile<rowsOther, colsOther>  const &dmatCompile);

    template <size_t index>
    DVector const &At() const;

    template <size_t index>
    DVector GetRow() const;

    template <size_t index>
    DVector GetCol() const;

    template <size_t index>
    DVector &At();

    template <size_t rowsOther, size_t colsOther>
    DMatrix Dot(DMatrixCompile<rowsOther, colsOther>  const &dmatCompile) const;

    DMatrix T() const;
};

template <size_t rows, size_t cols>
DMatrixCompile<rows, cols>::DMatrixCompile(double fill_value) : dmatRuntime(rows, cols, fill_value) 
{
    // pass
}


template <size_t rows, size_t cols>
template <size_t index>
DVector const &DMatrixCompile<rows, cols>::At() const
{
    static_assert(index < rows, ERROR_RANGE);
    return dmatRuntime[index];
}

template <size_t rows, size_t cols>
template <size_t index>
DVector &DMatrixCompile<rows, cols>::At() 
{
    static_assert(index < rows, ERROR_RANGE);
    return dmatRuntime[index];
}

template <size_t rows, size_t cols>
template <size_t index>
DVector DMatrixCompile<rows, cols>::GetRow() const
{
    static_assert(index < rows, ERROR_RANGE);
    return dmatRuntime.GetRow(index);
}

template <size_t rows, size_t cols>
template <size_t index>
DVector DMatrixCompile<rows, cols>::GetCol() const
{
    static_assert(index < cols, ERROR_RANGE);
    return dmatRuntime.GetCol(index);
}

template <size_t rows, size_t cols>
template <size_t rowsOther, size_t colsOther>
DMatrix DMatrixCompile<rows, cols>::operator+(DMatrixCompile<rowsOther, colsOther> const &dmatCompile)
{
    static_assert(rows == rowsOther, ERROR_LENGTH);
    static_assert(cols == colsOther, ERROR_LENGTH);
    return dmatRuntime + dmatCompile.dmatRuntime; 
}

template <size_t rows, size_t cols>
template <size_t rowsOther, size_t colsOther>
DMatrix DMatrixCompile<rows, cols>::operator-(DMatrixCompile<rowsOther, colsOther> const &dmatCompile)
{
    static_assert(rows == rowsOther, ERROR_LENGTH);
    static_assert(cols == colsOther, ERROR_LENGTH);
    return dmatRuntime - dmatCompile.dmatRuntime; 
}

template <size_t rows, size_t cols>
template <size_t rowsOther, size_t colsOther>
DMatrix DMatrixCompile<rows, cols>::operator*(DMatrixCompile<rowsOther, colsOther> const &dmatCompile)
{
    static_assert(rows == rowsOther, ERROR_LENGTH);
    static_assert(cols == colsOther, ERROR_LENGTH);
    return dmatRuntime * dmatCompile.dmatRuntime; 
}

template <size_t rows, size_t cols>
template <size_t rowsOther, size_t colsOther>
DMatrix DMatrixCompile<rows, cols>::operator/(DMatrixCompile<rowsOther, colsOther> const &dmatCompile)
{
    static_assert(rows == rowsOther, ERROR_LENGTH);
    static_assert(cols == colsOther, ERROR_LENGTH);
    return dmatRuntime / dmatCompile.dmatRuntime; 
}

template <size_t rows, size_t cols>
template <size_t rowsOther, size_t colsOther>
DMatrix DMatrixCompile<rows, cols>::Dot(DMatrixCompile<rowsOther, colsOther> const &dmatCompile) const
{
    static_assert(cols == rowsOther, ERROR_SIZE);
    /*
        нельзя обратиться напрямую к приватному полю, потому класс, инстанцированный с другими шаблонными параметрами, уже является другим 
    */
    return dmatRuntime.Dot(dmatCompile.GetDynamicMatrix());
}

template <size_t rows, size_t cols>
DMatrix DMatrixCompile<rows, cols>::T() const
{
    static_assert(cols == rows, ERROR_LENGTH);
    return dmatRuntime.T();
}
