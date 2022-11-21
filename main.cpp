#include "set.h"
#include "iterator.h"
#include <vector>

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
    for (auto elem : set)
    {
        std::cout << elem << std::endl;
    }
}