#include <gtest/gtest.h>
#include <cmath>

#include "dmatrix_compile.h"

extern bool CompareMatrices(DMatrix const &matrix, std::initializer_list<std::initializer_list<double>> const &init_list);

extern bool CompareVectors(DVector const &dvec, std::initializer_list<double> const &init_list);


TEST(TestFunctionalityDMatrixCompile, TestMatrixArithmeticOperator) 
{
    auto dmatSum = DMatrixCompile<3, 2>(1) + DMatrixCompile<3, 2>(2);

    /*
        Размерности не совпадают:
        DMatrixCompile<3, 2>(1) + DMatrixCompile<10, 10>(2);
        ERROR: static assertion failed
    */

    EXPECT_TRUE(CompareMatrices(dmatSum, {
        {3, 3},
        {3, 3},
        {3, 3}
    }));
}

TEST(TestFunctionalityDMatrixCompile, TestMatrixAccess) 
{

    DMatrixCompile<3, 2> dmatCompile1(3);

    EXPECT_EQ(dmatCompile1.At<0>()[0], 3);

    /*
        Доступ к строке за пределами матрицы:
        dmatCompile1.At<5>()[0]
        ERROR: static assertion failed
    */

    dmatCompile1.At<0>()[0] = 5;
    EXPECT_EQ(dmatCompile1.At<0>()[0], 5);
}

TEST(TestFunctionalityDMatrixCompile, TestMatrixDotProduct) 
{

    DMatrixCompile<3, 2> dmat1(1);
    DMatrixCompile<2, 5> dmat2(1);

    // 3x2 * 2x5 - 3x5
    auto dmatProd = dmat1.Dot(dmat2);

    /*
        Несоответствие размеров при матричном умножении:
        DMatrixCompile<3, 3> dmat1(1);
        ...
        ERROR: static assertion failed
    */

    EXPECT_TRUE(CompareMatrices(dmatProd, {
        {2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2}
    }));

}

TEST(TestFunctionalityDMatrixCompile, TestMatrixTransponse) 
{

    DMatrixCompile<3, 3> dmat1(1);
    dmat1.At<0>()[2] = 0;

    EXPECT_TRUE(CompareMatrices(dmat1.GetDynamicMatrix(), {
        {1, 1, 0},
        {1, 1, 1},
        {1, 1, 1}
    }));

    auto dmatTransponse = dmat1.T();

    /*
        Транспонирование неквадратной матрицы:
        DMatrixCompile<5, 3> dmat1(1);
        ...
        ERROR: static assertion failed
    */

    EXPECT_TRUE(CompareMatrices(dmatTransponse, {
        {1, 1, 1},
        {1, 1, 1},
        {0, 1, 1}
    }));
}

TEST(TestFunctionalityDMatrixCompile, TestGetRowAndCol) 
{

    DMatrixCompile<3, 4> dmat1(1);
    dmat1.At<2>()[1] = 0;

    auto vec1 = dmat1.GetRow<2>();

    EXPECT_TRUE(CompareVectors(vec1, { 1, 0, 1, 1 }));

    vec1 = dmat1.GetCol<1>();

    /*
        Изъятие строки или столбца за пределами матрицы:
        dmat1.GetRow<5>();
        dmat1.GetCol<5>();
        ERROR: static assertion failed
    */

    EXPECT_TRUE(CompareVectors(vec1, { 1, 1, 0 }));
}
