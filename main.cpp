#include <iostream>

#include "dvector.h"
#include "dmatrix.h"

// static IsEqualSize
// мб разделить реализацию на разные файлы
// создание матрицы из вектора с указание строки или столбца

int main() 
{
    DMatrix dmat1(3, 10, 4);
    DMatrix dmat2{
        {1, 2, 3},
        {4, 5, 6}
    };

    Print(dmat1);
    Print(dmat2);
}
