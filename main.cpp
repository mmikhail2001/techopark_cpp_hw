#include <iostream>

#include "dvector.h"
#include "dmatrix_compile.h"
#include "dmatrix.h"

int main() 
{
    DMatrix matrix = {
        {1, -2, 1},
        {2, 1, -1},
        {3, 2, -2}
    };
    DVector vector = {3, 4, 5};
    DMatrix newMatrix = 2 * vector.Dot(matrix.Inv());
    std::cout << newMatrix;
}
