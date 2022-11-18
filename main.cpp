#include "set.h"

int main(int argc, const char * argv[]) {
    Set<std::string> set;
    
    char op;
    std::string data;
    while (std::cin >> op >> data)
    {
        switch (op)
        {
            case '+':
            {
                set.Insert(data);
                break;
            }
            case '-':
            {
                set.Erase(data);
                break;
            }
            case '?':
            {
                std::cout << (set.Has(data) ? "YES" : "NO") << std::endl;
            }
        }
    }
    return 0;
}