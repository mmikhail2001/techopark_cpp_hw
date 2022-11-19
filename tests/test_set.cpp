#include <gtest/gtest.h>
#include <sstream>
#include "set.h"

template <typename T, typename Comparator>
void TestInsertErase(Set<T, Comparator> &set, std::istream &in)
{
	char op;
    T data;
	while (in >> op >> data)
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
        }
    }
}

TEST(AvlTree, TestBasicsFunctional)
{
	Set<int, std::greater<int>> set;
	// fill 1..9, remove odd
	std::string strTest = "+ 1 + 1 + 2 + 2 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 - 2 - 4 - 6 - 8 + -20 + 20";
	std::istringstream stream(strTest);
	TestInsertErase(set, stream);
	EXPECT_TRUE (set.Has(1));
	EXPECT_FALSE(set.Has(2));
	EXPECT_TRUE (set.Has(3));
	EXPECT_FALSE(set.Has(4));
	EXPECT_TRUE (set.Has(5));
	EXPECT_FALSE(set.Has(6));
	EXPECT_TRUE (set.Has(7));
	EXPECT_FALSE(set.Has(8));
	EXPECT_TRUE (set.Has(9));
	EXPECT_TRUE (set.Has(-20));
	EXPECT_TRUE (set.Has(20));
}