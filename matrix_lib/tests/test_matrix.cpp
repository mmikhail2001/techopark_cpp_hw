#include <gtest/gtest.h>
#include <cmath>

#include "dmatrix.h"

const double EPS = 0.000001;

bool CompareMatrices(DMatrix const &matrix, std::initializer_list<std::initializer_list<double>> const &init_list)
{
    for (size_t i = 0; i < matrix.nRows(); ++i)
    {
        for (size_t j = 0; j < matrix.nCols(); ++j)
        {
            if (std::abs(matrix[i][j] - *((init_list.begin() + i)->begin() + j)) > EPS)
            {
                return false;
            }
        }
    }
    return true;
}

bool CompareVectors(DVector const &dvec, std::initializer_list<double> const &init_list)
{
    for (size_t i = 0; i < dvec.Size(); ++i)
    {
        if (std::abs(dvec[i] - *(init_list.begin() + i)) > EPS)
        {
            return false;
        }
    }
    return true;
}


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
}

TEST(TestCreateDMatrix, TestConstructorsSpecifyRowOrCol) 
{
    DMatrix dmat(DVector{2, 4, 6, 8}); // ORIENT::ROW by default

    EXPECT_EQ(dmat.nRows(), 1);
    EXPECT_EQ(dmat.nCols(), 4);

    EXPECT_TRUE(CompareMatrices(dmat, { 
            {2, 4, 6, 8}
        }));

    dmat = DMatrix(DVector{2, 4, 6, 8}, ORIENT::COL);
    EXPECT_EQ(dmat.nRows(), 4);
    EXPECT_EQ(dmat.nCols(), 1);
    EXPECT_TRUE(CompareMatrices(dmat, { 
            {2},
            {4},
            {6},
            {8}
        }));
}

TEST(TestCreateDMatrix, TestConstructorsTemplate) 
{
    DMatrix dmat = DMatrix::Create<2, 3>(3);
    EXPECT_EQ(dmat.nRows(), 2);
    EXPECT_EQ(dmat.nCols(), 3);

    EXPECT_TRUE(CompareMatrices(dmat, { 
            {3, 3, 3},
            {3, 3, 3}
        }));
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

TEST(TestFunctionalityDMatrix, TestPushPopRow)
{
    DMatrix dmat;
    EXPECT_THROW(dmat.PopRowBack(), std::runtime_error);
    dmat.PushRowBack({1, 2, 3});
    
    DVector dvec{4, 5, 6};
    dmat.PushRowBack(dvec);

    EXPECT_THROW(dmat.PushRowBack({20, 20}), std::runtime_error);
    DVector dvec2{4, 4, 4, 4};
    EXPECT_THROW(dmat.PushRowBack(dvec2), std::runtime_error);

    
    EXPECT_TRUE(CompareMatrices(dmat, { 
            {1, 2, 3},
            {4, 5, 6}
        }));

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
TEST(TestFunctionalityDMatrix, TestPushPopRow_WithChangeDimensity)
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

TEST(TestFunctionalityDMatrix, TestPushPopCol)
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

TEST(TestFunctionalityDMatrix, TestPushPopMix)
{
    DMatrix dmat{
        {1, 2, 3},
        {4, 5, 6},
    };
    dmat.PushColBack({7, 7});
    dmat.PushRowBack({8, 8, 8, 100});

    EXPECT_TRUE(CompareMatrices(dmat, { 
            {1, 2, 3, 7},
            {4, 5, 6, 7},
            {8, 8, 8, 100},
        }));


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

    EXPECT_TRUE(CompareMatrices(dmat, { 
            {1, 2, 3, 4, 5, 6, 7, 8}
        }));

}

TEST(TestFunctionalityDMatrix, TestGetRowColDiag)
{
    // GetCol
    DMatrix dmat{
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10}
    };

    DVector dvec1 = dmat.GetCol(0);
    EXPECT_EQ(dvec1[0], dmat[0][0]);
    EXPECT_EQ(dvec1[1], dmat[1][0]);
    
    dvec1[0] = 100;
    EXPECT_NE(dvec1[0], dmat[0][0]);

    dmat[1][0] = 600;
    EXPECT_NE(dvec1[1], dmat[1][0]);

    DVector dvec2 = dmat.GetCol(4);
    EXPECT_EQ(dvec2.Size(), 2);
    EXPECT_EQ(dvec2[0], dmat[0][4]);
    EXPECT_EQ(dvec2[1], dmat[1][4]);

    // GetRow
    DVector dvec3 = dmat.GetRow(1);
    EXPECT_EQ(dvec3.Size(), 5);
    EXPECT_EQ(dvec3[0], dmat[1][0]);
    EXPECT_EQ(dvec3[4], dmat[1][4]);
    
    // GetDiag
    DVector dvec4 = dmat.GetDiag();
    EXPECT_EQ(dvec4.Size(), 2);
    EXPECT_EQ(dvec4[0], dmat[0][0]);
    EXPECT_EQ(dvec4[1], dmat[1][1]);

    DMatrix dmat2{
        {10},
        {20}
    };

    DVector dvec5 = dmat2.GetDiag();
    EXPECT_EQ(dvec5.Size(), 1);
    EXPECT_EQ(dvec5[0], dmat2[0][0]);
    
    DMatrix dmat3{
        {3, 5},
        {7, 9}
    };

    DVector dvec6 = dmat3.GetDiag();
    EXPECT_EQ(dvec6.Size(), 2);
    EXPECT_EQ(dvec6[0], dmat3[0][0]);
    EXPECT_EQ(dvec6[1], dmat3[1][1]);
}

TEST(TestFunctionalityDMatrix, TestArithmeticOperators)
{
    DMatrix dmat1{
        {1, 2, 3},
        {6, 7, 8}
    };
    DMatrix dmat2{
        {11, 12, 13},
        {14, 15, 16}
    };
    DMatrix dmatErr1{
        {1, 1},
        {1, 1}
    };

    DMatrix dmatErr2;
    DMatrix dmatErr3{
        {2, 4, 6}
    };
    DMatrix dmat3 = dmat1 * 2 + dmat2 / 1;

    EXPECT_THROW(dmat3 + dmatErr1, std::runtime_error);
    EXPECT_THROW(dmat3 *= dmatErr1, std::runtime_error);
    EXPECT_THROW(dmat3 -= dmatErr2, std::runtime_error);
    EXPECT_THROW(dmat3 / dmatErr3, std::runtime_error);

    EXPECT_TRUE(CompareMatrices(dmat3, { 
            {13, 16, 19}, 
            {26, 29, 32} 
            }));

}

TEST(TestFunctionalityDMatrix, TestDotProductOfMatrices)
{
    DMatrix dmat1{
        {1, 2, 3},
        {4, 5, 6}
    };

    DMatrix dmat2{
        {2, 3},
        {4, 5},
        {6, 7}
    };

    DMatrix dmatRes = dmat1.Dot(dmat2);
    EXPECT_EQ(dmatRes.nRows(), 2);
    EXPECT_EQ(dmatRes.nCols(), 2);

    EXPECT_TRUE(CompareMatrices(dmatRes, { {28, 34}, {64, 79} }));
}


TEST(TestFunctionalityDMatrix, TestDotProductBetweenMatrixAndVector)
{
    // matrix * vector

    DMatrix dmat1{
        {1, 2, 3},
        {4, 5, 6}
    };

    DVector dvec1{1, 2, 3};
    DVector dvecErr{1, 2, 3, 4};

    EXPECT_TRUE(CompareVectors(dmat1.Dot(dvec1), { 14, 32 }));
    EXPECT_THROW(dmat1.Dot(dvecErr), std::runtime_error);

    // vector * matrix

    DVector dvec2{1, 2};
    EXPECT_TRUE(CompareVectors(dvec2.Dot(dmat1), { 9, 12, 15 }));

    DMatrix dmatErr{
        {1, 2},
        {3, 4},
        {5, 6}
    };
    EXPECT_THROW(dvec2.Dot(dmatErr), std::runtime_error);
}

TEST(TestFunctionalityDMatrix, TestAddSubVectorAndNum)
{
    DMatrix dmat{
        {1, 2, 3},
        {4, 5, 6}
    };

    dmat.AddNum(1);
    EXPECT_TRUE(CompareMatrices(dmat, {
        {2, 3, 4},
        {5, 6, 7}
    }));

    DVector dvec1{10, 15};
    dmat.AddVec(dvec1, ORIENT::COL);
    EXPECT_TRUE(CompareMatrices(dmat, {
        {12, 13, 14},
        {20, 21, 22}
    }));

    DVector dvec2{10, 10, 0};
    dmat.SubVec(dvec2, ORIENT::ROW); // but ORIENT::ROW by default
    EXPECT_TRUE(CompareMatrices(dmat, {
        {2,  3,  14},
        {10, 11, 22}
    }));

    EXPECT_THROW(dmat.AddVec({1}, ORIENT::ROW), std::runtime_error);
    EXPECT_THROW(dmat.AddVec({1}, ORIENT::COL), std::runtime_error);
}

TEST(TestFunctionalityDMatrix, TestTransponse)
{
    DMatrix dmat{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    DMatrix dmatT = dmat.T();

    EXPECT_TRUE(CompareMatrices(dmatT, {
        {1,  4,  7},
        {2, 5, 8},
        {3, 6, 9}
    }));

    EXPECT_THROW(DMatrix({{1, 2}}).T(), std::runtime_error);
}

TEST(TestFunctionalityDMatrix, TestEraseByIndexRow)
{
    // Erase row
    DMatrix dmat{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    dmat.EraseByIndex(0);
    EXPECT_TRUE(CompareMatrices(dmat, {
        {4, 5, 6},
        {7, 8, 9}
    }));
    dmat.EraseByIndex(1);
    EXPECT_TRUE(CompareMatrices(dmat, {
        {4, 5, 6}
    }));


    EXPECT_EQ(dmat.nRows(), 1);
    EXPECT_EQ(dmat.nCols(), 3);

    EXPECT_THROW(dmat.EraseByIndex(1), std::runtime_error);
    dmat.EraseByIndex(0);
    EXPECT_THROW(dmat.EraseByIndex(0), std::runtime_error);

    EXPECT_EQ(dmat.nRows(), 0);
    EXPECT_EQ(dmat.nCols(), 0);
    EXPECT_EQ(dmat.Capacity(), 0);
}

TEST(TestFunctionalityDMatrix, TestEraseByIndexCol)
{
    DMatrix dmat = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    dmat.EraseByIndex(1, ORIENT::COL);
    EXPECT_TRUE(CompareMatrices(dmat, {
        {1, 3},
        {4, 6},
        {7, 9}
    }));

    EXPECT_EQ(dmat.nRows(), 3);
    EXPECT_EQ(dmat.nCols(), 2);

    dmat.EraseByIndex(0, ORIENT::COL);
    EXPECT_TRUE(CompareMatrices(dmat, {
        {3},
        {6},
        {9}
    }));

    dmat.EraseByIndex(0, ORIENT::COL);
    EXPECT_TRUE(CompareMatrices(dmat, {{}}));

    EXPECT_THROW(dmat.EraseByIndex(0, ORIENT::COL), std::runtime_error);
    EXPECT_THROW(dmat.EraseByIndex(200, ORIENT::COL), std::runtime_error);
}

TEST(TestFunctionalityDMatrix, TestDeterminant)
{
    DMatrix dmat1 = {
        {3, 5},
        {1, 5}
    };
    EXPECT_EQ(dmat1.Det(), 10);

    DMatrix dmat2 = {
        {3, 5, 2},
        {1, 5, 6},
        {5, 7, 2}
    };
    EXPECT_EQ(dmat2.Det(), 8);

    DMatrix dmat3 = {
        {3, 5, 2, 7},
        {1, 5, 6, 4},
        {5, 7, 2, 9},
        {5, 7, 2, 5}
    };
    EXPECT_EQ(dmat3.Det(), -32);
}

TEST(TestFunctionalityDMatrix, TestInvertibleMatrix)
{
    DMatrix dmat1 = {
        {1, -2, 1},
        {2, 1, -1},
        {3, 2, -2}
    };

    DMatrix dmatInv = dmat1.Inv();

    EXPECT_EQ(dmatInv.nRows(), 3);
    EXPECT_EQ(dmatInv.nCols(), 3);

    EXPECT_TRUE(CompareMatrices(dmatInv, {
        {0, 2, -1},
        {-1, 5, -3},
        {-1, 8, -5}
    }));

    DMatrix dmat2 = {
        {1, 1, 1, -1},
        {1, -2, 1, -1},
        {1, 1, 3, 1},
        {1, 1, 1, -4}
    };

    dmatInv = dmat2.Inv();

    EXPECT_TRUE(CompareMatrices(dmatInv, {
        {11./6, 1./3, -1./2, -2./3},
        {1./3, -1./3, 0, 0},
        {-5./6, 0, 1./2, 1./3},
        {1./3, 0, 0, -1./3}
    }));

    DMatrix dmatErr1 = {
        {1, -2, 1},
        {2, 1, -1},
    };

    // определитель = 0, обратной матрицы не существует
    DMatrix dmatErr2 = {
        {1, 2},
        {2, 4},
    };

    EXPECT_THROW(dmatErr1.Inv(), std::runtime_error);
    EXPECT_TRUE(dmatErr2.Inv().Empty());
}

TEST(TestFunctionalityDMatrix, TestSliceOperatorRow)
{
    DMatrix dmat = {
        {1,   2,  3,  4,  5},
        {6,   7,  8,  9,  10},
        {11,  12, 13, 14, 15},
        {16,  17, 18, 19, 20},
        {21,  22, 23, 24, 25}
    };

    DMatrix dmatSlice1 = dmat(1, 3);
    EXPECT_TRUE(CompareMatrices(dmatSlice1, {
        {6,   7,  8,  9,  10},
        {11,  12, 13, 14, 15}
    }));

    DMatrix dmatSlice2 = dmat(0, dmat.nRows());

    EXPECT_TRUE(CompareMatrices(dmatSlice2, {
        {1,   2,  3,  4,  5},
        {6,   7,  8,  9,  10},
        {11,  12, 13, 14, 15},
        {16,  17, 18, 19, 20},
        {21,  22, 23, 24, 25}
    }));

    DMatrix dmatSlice3 = dmat(0, dmat.nRows(), 2);

    EXPECT_TRUE(CompareMatrices(dmatSlice3, {
        {1,   2,  3,  4,  5},
        {11,  12, 13, 14, 15},
        {21,  22, 23, 24, 25}
    }));

    DMatrix dmatSlice4 = dmat(0, dmat.nRows(), -2);

    EXPECT_TRUE(CompareMatrices(dmatSlice4, {
        {21,  22, 23, 24, 25},
        {11,  12, 13, 14, 15},
        {1,   2,  3,  4,  5}
    }));

    EXPECT_THROW(dmat(4, 1), std::runtime_error);
    EXPECT_THROW(dmat(2, 2), std::runtime_error);
    EXPECT_THROW(dmat(100, 103), std::runtime_error);
    EXPECT_THROW(dmat(0, 100), std::runtime_error);

    DMatrix dmatSlice5 = dmat(0, 4, 0);
    EXPECT_TRUE(CompareMatrices(dmatSlice5, { {} }));
}

TEST(TestFunctionalityDMatrix, TestSliceOperatorCol)
{
    DMatrix dmat = {
        {1,   2,  3,  4,  5},
        {6,   7,  8,  9,  10},
        {11,  12, 13, 14, 15},
        {16,  17, 18, 19, 20},
        {21,  22, 23, 24, 25}
    };

    DMatrix dmatSlice1 = dmat(1, 3, 1, COL);
    EXPECT_TRUE(CompareMatrices(dmatSlice1, {
        {2,  3  },
        {7,  8  },
        {12, 13 },
        {17, 18 },
        {22, 23 }
    }));

    DMatrix dmatSlice2 = dmat(1, 4, 2, COL);

    EXPECT_TRUE(CompareMatrices(dmatSlice2, {
        {2,  4  },
        {7,  9  },
        {12, 14 },
        {17, 19 },
        {22, 24 }
    }));

    DMatrix dmatSlice3 = dmat(0, dmat.nCols(), -1, COL);

    EXPECT_TRUE(CompareMatrices(dmatSlice3, {
        {5, 4, 3, 2, 1},
        {10, 9, 8, 7, 6},
        {15, 14, 13, 12, 11},
        {20, 19, 18, 17, 16},
        {25, 24, 23, 22, 21}
    }));

    DMatrix dmatSlice4 = dmat(4, dmat.nCols(), 1, COL);

    EXPECT_TRUE(CompareMatrices(dmatSlice4, {
        {5},
        {10},
        {15},
        {20},
        {25}
    }));
}

TEST(TestFunctionalityDMatrix, TestSliceOperatorRowColMix)
{
    DMatrix dmat = {
        {1,   2,  3,  4,  5},
        {6,   7,  8,  9,  10},
        {11,  12, 13, 14, 15},
        {16,  17, 18, 19, 20},
        {21,  22, 23, 24, 25}
    };

    DMatrix dmatSlice1 = dmat(0, 3)(1, 3);

    EXPECT_TRUE(CompareMatrices(dmatSlice1, {
        {6,   7,  8,  9,  10},
        {11,  12, 13, 14, 15}
    }));

    // ROW = 0, COL = 1
    DMatrix dmatSlice2 = dmat(0, 4, 1, 0)(1, 3, 1, 1);
    EXPECT_TRUE(CompareMatrices(dmatSlice2, {
        {2, 3},
        {7, 8},
        {12, 13},
        {17, 18}
    }));

    DMatrix dmatSlice3 = dmat(1, 2, 1, COL)(3, 4);
    EXPECT_TRUE(CompareMatrices(dmatSlice3, {
        {17}
    }));
}

TEST(TestFunctionalityDMatrix, TestSliceCallMethod)
{
    DMatrix dmat = {
        {1,   2,  3,  4,  5},
        {6,   7,  8,  9,  10},
        {11,  12, 13, 14, 15},
        {16,  17, 18, 19, 20},
        {21,  22, 23, 24, 25}
    };

    DMatrix dmatSlice1 = dmat.SliceRow(0, 4).SliceCol(1, 5, 2);

    EXPECT_TRUE(CompareMatrices(dmatSlice1, {
        {2,     4 },
        {7,     9 },
        {12,    14},
        {17,    19}
    }));
}

