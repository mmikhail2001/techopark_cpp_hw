#include "set.h"
#include "iterator.h"

/*
1. HAS -> FIND
2. FILL LIST AFTER INSERT AND ERASE
3. ITERATOR CHECK LIST

*/


int main(int argc, const char * argv[]) 
{
    Set<int> set;
    set.insert(70);
    set.insert(90 );
    set.insert(110);
    set.insert(3);
    set.insert(130);
    set.insert(80 );
    set.insert(120);
    set.insert(100);
    set.insert(-7);

    // set.erase(7);
    // set.erase(8);
    set.erase(70);
    // set.erase(13);
    set.erase(100);
    // set.erase(11);

    // auto it = set.lower_bound(-8);

    for (auto elem : set)
    {
        std::cout << elem << " ";
    }
    puts("");

    for (auto it = set.begin(); it != set.end(); it++)
    {
        std::cout << *it << " ";
    }
    puts("");

    // std::cout << set.find(8)->data << "\n";
    // std::cout << set.find(11)->data << "\n";
    // std::cout << set.find(12)->data << "\n";
    // std::cout << set.find(123) << "\n";



    // for (auto elem : set)
    // {
    //     std::cout << elem << " ";
    // }

    // std::cout << "size = " << set.size() << "\n";

    // set.Erase(7);
    // set.Erase(11);

    // std::cout << "size = " << set.size() << "\n";

    // std::cout << "\n";
    // for (auto it = set.begin(); it != set.end(); it++)
    // {
    //     std::cout << *it << " ";
    // }

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