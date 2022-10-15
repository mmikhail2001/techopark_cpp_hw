#include <iostream>

#include "dvector.h"
#include "dmatrix.h"

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
