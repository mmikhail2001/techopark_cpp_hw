#include <gtest/gtest.h>

#include "dmatrix.h"

TEST(TestCreateDMatrix, TestConstructorsSpecifySizeAndInitList) 
{
    DMatrix dmat1(3, 10, 4);
    DMatrix dmat2{
        {1, 2, 3},
        {4, 5, 6}
    };
    EXPECT_EQ(dmat1.nCols(), 10);
    EXPECT_EQ(dmat1.nRows(), 3);
    EXPECT_EQ(dmat1[0][0], 4);
    EXPECT_EQ(dmat1[2][9], 4);
    
    EXPECT_EQ(dmat2.nCols(), 3);
    EXPECT_EQ(dmat2.nRows(), 2);
    EXPECT_EQ(dmat2[0][0], 1);
    EXPECT_EQ(dmat2[0][1], 2);
    EXPECT_EQ(dmat2[1][2], 6);
}

TEST(TestCreateDMatrix, TestConstructorsDVectorsAndCopy) 
{
    const size_t ndvecs = 2;
    DVector *dvecs = new DVector[ndvecs]{ DVector(2, 7), DVector{3, 8} };

    DMatrix dmat1(dvecs, ndvecs);
    DMatrix dmat2(dmat1);

    EXPECT_NE(dmat1[0].CBegin(), dmat2[0].CBegin());
    EXPECT_EQ(dmat1[1][0], dmat2[1][0]);

    EXPECT_EQ(dmat1[1][0], 3);
    EXPECT_EQ(dmat1[0][1], 7);

    DMatrix dmat3(DVector{2, 4, 6, 8});
    EXPECT_EQ(dmat3.nRows(), 1);
    EXPECT_EQ(dmat3.nCols(), 4);
    EXPECT_EQ(dmat3[0][2], 6);
}

TEST(TestCreateDMatrix, TestOperatorAssign) 
{
    DMatrix dmat1{
        {1, 2, 3},
        {4, 5, 6},
    };

    DMatrix dmat2 = dmat1;
    EXPECT_NE(dmat1[0].CBegin(), dmat2[0].CBegin());
    EXPECT_EQ(dmat1.nRows(), dmat2.nRows());
    EXPECT_EQ(dmat1.nCols(), dmat2.nCols());

    EXPECT_EQ(dmat1[1][0], dmat2[1][0]);
    EXPECT_EQ(dmat1[1][0], 4);
}

TEST(TestErrorHandling, TestAccessAndCreateNotRectangleMatrix)
{
    EXPECT_THROW( DMatrix dmat({ {1, 2}, {4, 5, 6} }), std::runtime_error);
    EXPECT_THROW( DMatrix dmat({ {1}, {} }), std::runtime_error);

    DVector *dvecs = new DVector[2]{ DVector{1, 2}, DVector{3, 4, 5} };
    EXPECT_THROW(DMatrix dmat(dvecs, 2), std::runtime_error);

    DMatrix dmat{ {1, 2, 3}, {4, 5, 6} };

    EXPECT_THROW(dmat[2][1], std::runtime_error);
    EXPECT_THROW(dmat[0][5], std::runtime_error);
}

TEST(TestErrorHandling, TestPushPopRow)
{
    DMatrix dmat;
    EXPECT_THROW(dmat.PopRowBack(), std::runtime_error);
    dmat.PushRowBack({1, 2, 3});
    
    DVector dvec{4, 5, 6};
    dmat.PushRowBack(dvec);

    EXPECT_THROW(dmat.PushRowBack({20, 20}), std::runtime_error);
    DVector dvec2{4, 4, 4, 4};
    EXPECT_THROW(dmat.PushRowBack(dvec2), std::runtime_error);

    
    EXPECT_EQ(dmat[0][1], 2);
    EXPECT_EQ(dmat[1][2], 6);
    EXPECT_EQ(dmat.nCols(), 3);
    EXPECT_EQ(dmat.nRows(), 2);

    dmat.PushRowBack({0, 0, 7});
    dmat.PushRowBack({0, 0, 7});
    dmat.PushRowBack({0, 0, 7});


    dmat.PopRowBack();
    dmat.PopRowBack();
    EXPECT_EQ(dmat.nRows(), 3);

    EXPECT_EQ(dmat.Capacity(), 8);
}

// размер матрицы зависит от ее содержимого
// без содержимого матрица не имеет размера и может в будущем принимать любые формы
TEST(TestErrorHandling, TestPushPopRow_WithChangeDimensity)
{
    DMatrix dmat;
    dmat.PushRowBack({0, 0, 7});
    dmat.PushRowBack({0, 0, 7});
    dmat.PushRowBack({0, 0, 7});
    EXPECT_EQ(dmat.nCols(), 3);
    EXPECT_EQ(dmat.nRows(), 3);
    EXPECT_EQ(dmat.Capacity(), 4);

    dmat.PopRowBack();
    dmat.PopRowBack();
    dmat.PopRowBack();
    EXPECT_THROW(dmat.PopRowBack(), std::runtime_error);

    dmat.PushRowBack({1});

    EXPECT_EQ(dmat.nCols(), 1);
    EXPECT_EQ(dmat.nRows(), 1);
    EXPECT_EQ(dmat.Capacity(), 1);
}

TEST(TestErrorHandling, TestPushPopCol)
{
    DMatrix dmat;
    dmat.PushColBack({1, 2, 3});
    EXPECT_EQ(dmat.nCols(), 1);
    EXPECT_EQ(dmat.nRows(), 3);

    EXPECT_THROW(dmat.PushColBack({1, 1}), std::runtime_error);

    DVector dvec{4, 5, 6};
    dmat.PushColBack(dvec);

    EXPECT_EQ(dmat.nCols(), 2);
    EXPECT_EQ(dmat.nRows(), 3);

    dmat.PopColBack();
    dmat.PopColBack();
    EXPECT_THROW(dmat.PopColBack(), std::runtime_error);

    dmat.PushColBack({1, 2});
    dmat.PushColBack({3, 4});

    EXPECT_EQ(dmat.nCols(), 2);
    EXPECT_EQ(dmat.nRows(), 2);
}

TEST(TestErrorHandling, TestPushPopMix)
{
    DMatrix dmat{
        {1, 2, 3},
        {4, 5, 6},
    };
    dmat.PushColBack({7, 7});
    dmat.PushRowBack({8, 8, 8, 100});

    EXPECT_EQ(dmat[2][3], 100);

    EXPECT_EQ(dmat.nCols(), 4);
    EXPECT_EQ(dmat.nRows(), 3);

    dmat.PopColBack();
    dmat.PopRowBack();
    dmat.PopRowBack();

    EXPECT_EQ(dmat.nCols(), 3);
    EXPECT_EQ(dmat.nRows(), 1);

    dmat.PopRowBack();
    EXPECT_THROW(dmat.PopRowBack(), std::runtime_error);

    EXPECT_EQ(dmat.nCols(), 0);
    EXPECT_EQ(dmat.nRows(), 0);

    dmat = {{1, 2, 3, 4, 5, 6, 7}};
    dmat.PushColBack({8});
    EXPECT_EQ(dmat[0][0], 1);
    EXPECT_EQ(dmat[0][7], 8);
}