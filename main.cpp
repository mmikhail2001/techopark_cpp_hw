#include <iostream>

#include "dvector.h"

int main() 
{
    DVector dv1{1, 2, 3, 4};
    DVector dv2(dv1);
    dv2.PushBack(23);
    Print(dv1, "dv1");
    Print(dv2, "dv2");

    DVector dv3 = std::move(dv1);
    // DVector dv3 = dv1;
    dv3.PopBack();
    Print(dv1, "dv1");
    Print(dv3, "dv3");
}
