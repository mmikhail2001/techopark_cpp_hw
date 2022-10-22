#include <gtest/gtest.h>

#include "dvector.h"

extern bool CompareVectors(DVector const &dvec, std::initializer_list<double> const &init_list);

TEST(TestCreateDVector, TestDefaultConstructor) 
{
    DVector dvec1;
    dvec1.PushBack(1);
    dvec1.PushBack(2);
    dvec1.PushBack(3);
    dvec1.PushBack(4);
    EXPECT_EQ(dvec1.Size(), 4);   
    EXPECT_EQ(dvec1.Capacity(), 4); 
    EXPECT_TRUE(CompareVectors(dvec1, {1, 2, 3, 4}));   
}

TEST(TestCreateDVector, TestConstructorInitializerList) 
{
    DVector dvec2{5, 6, 7, 8, 9};
    EXPECT_EQ(dvec2.Size(), 5);   
    EXPECT_EQ(dvec2.Capacity(), 5);   
    EXPECT_TRUE(CompareVectors(dvec2, {5, 6, 7, 8, 9}));  
}

TEST(TestCreateDVector, TestConstructorInputSizeAndFillValue) 
{
    DVector dvec3(10, 99);
    EXPECT_EQ(dvec3.Size(), 10);
    EXPECT_EQ(dvec3.Capacity(), 10);
    EXPECT_TRUE(CompareVectors(dvec3, {99, 99, 99, 99, 99, 99, 99, 99, 99, 99}));    
}

TEST(TestCreateDVector, TestCopyConstructor) 
{
    DVector dvec1{5, 6, 7, 8, 9};
    DVector dvec4(dvec1);
    EXPECT_NE(dvec4.CBegin(), dvec1.CBegin());
    EXPECT_EQ(dvec4.Size(), dvec1.Size());
    EXPECT_EQ(dvec4.Capacity(), dvec1.Capacity());
    EXPECT_EQ(dvec4[0], dvec1[0]);
    EXPECT_EQ(dvec4[3], dvec1[3]);
}

TEST(TestCreateDVector, TestConstructorPointersBeginEnd) 
{
    const size_t size = 5;
    double vec[size] = {1, 2, 3, 4, 5};
    // + 1, т.к. end - указатель на элемент после последнего
    DVector dvec(vec, &vec[size - 2] + 1);
    EXPECT_EQ(dvec.Size(), size - 1);
    EXPECT_EQ(dvec[size - 2], vec[size - 2]);
    EXPECT_THROW(dvec[size - 1], std::runtime_error);

    DVector dvec1(dvec.CBegin(), dvec.CEnd());
    EXPECT_NE(dvec1.CBegin(), dvec.CBegin());
    EXPECT_EQ(dvec1.Size(), dvec.Size());

    EXPECT_TRUE(CompareVectors(dvec1, {1, 2, 3, 4}));  
}

TEST(TestCreateDVector, TestImplicitConstructor)
{
    auto func = [](DVector const &dvec) -> size_t
    { return dvec.Size(); };

    EXPECT_EQ(func({1, 2, 3}), 3);
    // func(5); ERROR: no match for call
}

TEST(TestCreateDVector, TestOperatorAssign) 
{
    DVector dvec1 = {5, 6, 7, 8, 9};
    DVector dvec2({1, 2, 3});
    dvec1 = dvec2;
    EXPECT_NE(dvec1.CBegin(),   dvec2.CBegin());
    EXPECT_EQ(dvec1.Size(),     dvec2.Size());
    EXPECT_EQ(dvec1.Capacity(), dvec2.Capacity());

    dvec1[0] = 10;
    dvec1.PushBack(30);

    dvec2 = std::move(dvec1);
    EXPECT_EQ(dvec1.CBegin(), nullptr);
    EXPECT_EQ(dvec1.Size(), 0);
    EXPECT_EQ(dvec1.Capacity(), 0);

    EXPECT_EQ(dvec2.Size(), 4);

    dvec2 = DVector(20, 5);
    EXPECT_EQ(dvec2.Size(), 20);
    
    dvec2.Clear();
    dvec1 = dvec2;
    EXPECT_EQ(dvec1.Size(), 0);
    EXPECT_EQ(dvec2.Size(), 0);
}

TEST(TestErrorHandling, TestAccess)
{
    DVector dvec1{5, 6, 7, 8, 9};
    EXPECT_EQ(dvec1[1], 6);
    dvec1[1] = 60;
    EXPECT_EQ(dvec1[1], 60);

    EXPECT_THROW(dvec1[5], std::runtime_error);
    EXPECT_THROW(dvec1[5] = 12, std::runtime_error);
    EXPECT_THROW(dvec1[-1], std::runtime_error);

    const DVector dvec2(dvec1);
    EXPECT_EQ(dvec2[1], 60);
    // dvec2[1] = 600; ERROR: assignment of read-only location
} 

TEST(TestFunctionalityDVector, TestFindErase)
{
    DVector dvec{5, 6, 7, 8, 9};
    DVector dvec1(dvec);
    EXPECT_EQ(dvec1.Find(6), &dvec1[1]);
    EXPECT_EQ(dvec1.Find(10), nullptr);

    EXPECT_EQ(dvec1.Erase(dvec1.Find(10)), nullptr);

    EXPECT_EQ(dvec1.Erase(dvec1.Find(7)), &dvec1[2]);
    EXPECT_EQ(dvec1.Size(), 4);

    dvec1 = dvec;
    EXPECT_EQ(dvec1.Erase(dvec1.Find(9)), &dvec1[3]);
    EXPECT_EQ(dvec1.Size(), 4);
} 

TEST(TestFunctionalityDVector, TestPushPopFrontBack)
{
    DVector dvec;
    EXPECT_THROW(dvec.PopBack(), std::runtime_error);
    dvec.PushBack(1);
    dvec.PushBack(2);
    dvec.PushBack(3);
    EXPECT_EQ(dvec.Back(), 3);
    EXPECT_EQ(dvec.Front(), 1);
}

TEST(TestFunctionalityDVector, TestArithmeticOperators)
{
    DVector dvec1{1, 2, 3};
    DVector dvec2{5, 6, 7};
    (dvec1 *= dvec2) += dvec1;

    EXPECT_TRUE(CompareVectors(dvec1, {10, 24, 42}));  
    
    DVector dvec3{2, 4, 8};
    DVector dvec4{4, 5, 6};

    DVector dvec5 = (dvec3 * 2 * dvec4) + dvec3 / 2;

    /*
        { 4 * 4; 8 * 5; 16 * 6 }
        +
        { 1; 2; 4 }
        =
        { 17; 42; 100 }
    */

    dvec5 *= 2;

    EXPECT_EQ(dvec5[0], 17 * 2);
    EXPECT_EQ(dvec5[1], 42 * 2);
    EXPECT_EQ(dvec5[2], 100 * 2);

    DVector dvec6{3, 4};
    DVector dvec7{10, 2};
    double resDot = dvec6.Dot(dvec7);
    EXPECT_EQ(resDot, 38);
}

TEST(TestFunctionalityDVector, TestAddSubNum)
{
    DVector dvec{1, 2, 3};
    dvec.AddNum(2);
    EXPECT_TRUE(CompareVectors(dvec, {3, 4, 5}));
    dvec.SubNum(3);
    EXPECT_TRUE(CompareVectors(dvec, {0, 1, 2}));
}

TEST(TestFunctionalityDVector, TestSliceOperator)
{
    DVector dvec{1, 2, 3, 4, 5, 6, 7};

    EXPECT_TRUE(CompareVectors(dvec(0, 5), {1, 2, 3, 4, 5}));
    EXPECT_TRUE(CompareVectors(dvec(0, dvec.Size(), -1), {7, 6, 5, 4, 3, 2, 1}));

    EXPECT_TRUE(CompareVectors(dvec(3, 4), {4}));
    
    EXPECT_THROW(dvec(4, 4), std::runtime_error);
    EXPECT_THROW(dvec(5, 4), std::runtime_error);
    EXPECT_THROW(dvec(1, 400), std::runtime_error);
    EXPECT_THROW(dvec(20, 22), std::runtime_error);
    
}
