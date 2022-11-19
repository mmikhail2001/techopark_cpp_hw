#include "set.h"
#include "iterator.h"

int main(int argc, const char * argv[]) 
{
    Set<int> set;
    set.Insert(7);
    set.Insert(8);
    set.Insert(9);
    set.Insert(10);
    set.Insert(11);



    for (auto elem : set)
    {
        std::cout << elem << " ";
    }

    std::cout << "size = " << set.size() << "\n";

    set.Erase(7);
    set.Erase(11);

    std::cout << "size = " << set.size() << "\n";

    std::cout << "\n";
    for (auto it = set.begin(); it != set.end(); it++)
    {
        std::cout << *it << " ";
    }

    // puts("\n\n0");
    // auto start  = --set.end();
    // puts("1");
    // auto end    = --set.begin();
    // puts("2");
    // for (; start != end; start--)
    // {
    //     std::cout << *start << " ";
    // }
    return 0;
}