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

using std::cout;
using std::endl;

template <class T>
void eq(T left, T right)
{
    if (left == right)
    {
        std::cout << "equal\n";
        return;
    }
    std::cout << "not equal\n";
    return;
}

int main(int argc, const char * argv[]) 
{
    Set<int> set = {1, 5, 3, 7, 34, 88, 23};

    auto it = set.begin();
    
    --it;
    
    ++it;

    cout << *it << endl;

    auto it2 = set.end();

    it2--;
    cout << *it2 << endl;
    it2++;
    it2++;
    it2++;

    it2--;
    cout << *it2 << endl;

    // set.erase(1);
    // set.erase(5);
    // set.erase(6);
    // set.erase(7);

    // for (auto elem : set)
    // {
    //     std::cout << elem << " ";
    // }
    // return 0;
}