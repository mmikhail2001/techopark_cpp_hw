#include "set.h"

void lolFunc(Set<int> &set)
{
    auto it1 = set.end();
    // std::cout << "lolFunc it1 = " << it1 << std::endl;
    if (it1)
    {
        std::cout << "it1 " << it1->data << "\n";
    }
    auto it2 = set.PrevInternal(it1);
    // std::cout << "lolFunc it2 = " << it2 << std::endl;
    if (it2)
    {
        std::cout << "it2 " << it2->data << "\n";
        auto it3 = set.PrevInternal(it2);
        if (it3)
        {
            std::cout << "it3 " << it3->data << "\n";
            auto it4 = set.PrevInternal(it3);
            if (it4)
            {
                std::cout << "it4 " << it4->data << "\n";
                auto it5 = set.PrevInternal(it4);
                if (it5)
                {
                    std::cout << "it5 " << it5->data << "\n";
                    auto it6 = set.PrevInternal(it5);
                    if (it6)
                    {
                        std::cout << "it6 " << it6->data << "\n";
                    }
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    Set<int> set;
    set.Insert(7);
    set.Insert(8);
    set.Insert(9);
    set.Insert(10);
    set.Insert(11);
    set.Insert(12);
    set.Insert(13);
    set.Insert(14);
    set.Insert(15);
    set.Insert(16);
    set.Insert(17);
    set.Insert(18);
    set.Insert(19);
    set.Insert(20);

    // for (auto it = set.begin(); it != set.end(); it = set.NextInternal(it))
    // {
    //     std::cout << it->data << " ";
    // }

    for (auto it = set.begin(); it != set.end(); it = it->Next())
    {
        std::cout << it->data << " ";
    }

    set.Erase(10);
    std::cout << "\n";

    for (auto it = set.begin(); it != set.end(); it = it->Next())
    {
        std::cout << it->data << " ";
    }

    set.Erase(7);
    set.Erase(12);
    set.Erase(8);
    std::cout << "\n";

    for (auto it = set.begin(); it != set.end(); it = it->Next())
    {
        std::cout << it->data << " ";
    }

    // set.Erase(8);
    // puts("----------");
    // lolFunc(set);

    // puts("----------");
    // set.Erase(10);
    // lolFunc(set);

    // puts("----------");
    // set.Erase(11);
    // lolFunc(set);
    
    // puts("----------");
    // set.Erase(7);
    // lolFunc(set);
    

    // puts("----------");
    // set.Erase(9);
    // lolFunc(set);

    // puts("----------");
    // std::cout << "до 12" << std::endl;
    // set.Erase(12);
    // std::cout << "после 12" << std::endl;
    // lolFunc(set);

    // puts("----------");
    // std::cout << "до ----" << std::endl;
    // std::cout.flush();
    // set.Erase(11);
    // std::cout << "после ---" << std::endl;
    // std::cout.flush();

    // lolFunc(set);
    
    // char op;
    // int data;
    // while (std::cin >> op >> data)
    // {
    //     switch (op)
    //     {
    //         case '+':
    //         {
    //             set.Insert(data);
    //             break;
    //         }
    //         case '-':
    //         {
    //             set.Erase(data);
    //             break;
    //         }
    //         case '?':
    //         {
    //             std::cout << (set.Has(data) ? "YES" : "NO") << std::endl;
    //         }
    //     }
    // }
    return 0;
}