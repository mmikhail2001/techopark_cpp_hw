#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <set>
#include "set.h"
#include "iterator.h"

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
                set.insert(data);
                break;
            }
            case '-':
            {
                set.erase(data);
                break;
            }
        }
    }
}

TEST(TestBasicsFunctional, Test_Insert_Erase_Find_LowerBound)
{
	Set<int> set;
    EXPECT_EQ(set.size(), 0);
    EXPECT_TRUE(set.empty());

	// fill 1..9, -20, 20
    // remove odd
	std::string strTest = "+ 1 + 1 + 2 + 2 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 - 2 - 4 - 6 - 8 -8 -8 + -20 + 20";
	std::istringstream stream(strTest);
	TestInsertErase(set, stream);

    EXPECT_EQ(set.size(), 7);
    EXPECT_TRUE(!set.empty());

	EXPECT_EQ(*(set.find(1)), 1);
	EXPECT_EQ(*(set.find(3)), 3);
	EXPECT_EQ(*(set.find(5)), 5);
	EXPECT_EQ(*(set.find(7)), 7);
	EXPECT_EQ(*(set.find(9)), 9);
	EXPECT_EQ(*(set.find(-20)), -20);
	EXPECT_EQ(*(set.find(20)), 20);

	EXPECT_EQ(*(set.lower_bound(2)), 3);
	EXPECT_EQ(*(set.lower_bound(9)), 9);
	EXPECT_EQ(*(set.lower_bound(-100)), -20);

    // EXPECT_EQ: отсутствие оператора == ... 
	EXPECT_TRUE(set.find(2) == set.end());
	EXPECT_TRUE(set.find(4) == set.end());
	EXPECT_TRUE(set.find(6) == set.end());
	EXPECT_TRUE(set.find(8) == set.end());

    set.erase(12345);
    for (auto elem : {1, 3, 5, 7, 9, -20})
    {
        set.erase(elem);
    }
    EXPECT_EQ(set.size(), 1);
    set.erase(20);
    EXPECT_EQ(set.size(), 0);
    set.erase(20);
    set.erase(2001);
}

TEST(TestBasicsFunctional, Test_Increment_Decrement_Begin_End)
{
    Set<int> set;
    EXPECT_TRUE(set.begin() == set.end());
    set = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    Set<int>::iterator it = set.find(40);
    EXPECT_TRUE(it != set.end());
    EXPECT_EQ(*it, 40);
    EXPECT_EQ(*(--it), 30);
    EXPECT_EQ(*(it++), 30);
    EXPECT_EQ(*(it), 40);
    EXPECT_EQ(*(++it), 50);
    EXPECT_TRUE(it == set.find(50));
    EXPECT_TRUE(it != set.find(70));
    EXPECT_EQ(*set.begin(), 10);
    EXPECT_EQ(*(++set.begin()), 20);
    // РАБОТАЕТ!!!
    EXPECT_EQ(*(--set.end()), 90);
}

TEST(TestBasicsFunctional, Test_Ctor_CopyCtor_AssignOperator)
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2(set1.begin(), set1.end());
    EXPECT_EQ(set1, set2);
    set2.erase(4);
    EXPECT_NE(set1, set2);
    
    set2 = set1;
    EXPECT_EQ(set1, set2);

    auto it1 = set1.find(2);
    auto it2 = set1.find(5);
    Set<int> set3(it1, it2);
    EXPECT_EQ(set3.size(), 3);
    auto it = set3.begin();
    for (auto elem : {2, 3, 4})
    {
        EXPECT_EQ(*it, elem);
        ++it;
    }
}

TEST(TestBasicsFunctional, Test_Compatibility_with_STL_Next_Advance_Prev)
{
    // for range выше
    Set<int> set = {1, 2, 3, 4, 5};
    Set<int>::iterator it = set.begin();
    std::advance(it, 3);
    EXPECT_EQ(*it, 4);
    auto it2 = std::next(it);
    EXPECT_EQ(*it2, 5);
    it2 = std::prev(it, 2);
    EXPECT_EQ(*it2, 2);
}

TEST(TestBasicsFunctional, Test_Compatibility_with_STL_Vector)
{
    std::vector<int> vec = {1, 2, 3, 4, 5};
    Set<int> set(vec.begin(), vec.end());
    auto it = set.begin();
    for (auto &&elem : vec)
    {
        EXPECT_EQ(*it, elem);
        ++it;        
    }
    
    int count = std::count_if(set.begin(), set.end(), [](int data)
    {
        return data % 2 == 0;
    });
    EXPECT_EQ(count, 2);

    // CORRECT ERROR:  no matching function for call to ‘Set<int>::insert(Set<int>::iterator&, const value_type&)’

    std::vector<int> vec2;
    std::copy(set.begin(), set.end(), std::back_inserter(vec2));
    size_t index = 0;
    for (auto &&elem : set)
    {
        EXPECT_EQ(elem, vec2[index]);
        index++;
    }


    /*
    
        CORRECT ERROR: no matching function for call to ‘Set<int>::insert(Set<int>::iterator&, const value_type&)’
        Set<int> set2 = {-2, -1, 0, 6};
        std::copy(set.begin(), set.end(), std::inserter(set2, set2.begin()));
    */
    /*
        нельзя, т.к. итератор константный
        auto square = [](int &data)
        {
            data * data;
        };
        std::for_each(set.begin(), set.end(), square);
    
    */
}

