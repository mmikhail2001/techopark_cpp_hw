#include "set.h"
#include "iterator.h"
#include <vector>

/*
    Задачи
    - Маркерный Node - указатель на узел после последнего
        - Сделали +-
    - Маркерный Node - указатель на узел перед первым (для реверсивного итератра)
        - Не сделали
*/


int main(int argc, const char * argv[]) 
{
    Set<int> set = {1, 5, 3, 7, 34, 88, 23};

    set.erase(1);
    set.erase(5);
    set.erase(6);
    set.erase(7);

    for (auto elem : set)
    {
        std::cout << elem << " ";
    }
    return 0;
}