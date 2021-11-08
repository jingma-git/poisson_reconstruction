#pragma once
#include <Eigen/Eigen>

namespace egl
{
    template <typename DerivedV, typename DerivedF>
    bool make_cube(
        Eigen::PlainObjectBase<DerivedV> &V,
        Eigen::PlainObjectBase<DerivedF> &F)
    {
        V.resize(8, 3);
        F.resize(12, 3);

        V(0, 0) = 0;
        V(0, 1) = 0;
        V(0, 2) = 0;

        V(1, 0) = 0;
        V(1, 1) = 1;
        V(1, 2) = 0;

        V(2, 0) = 1;
        V(2, 1) = 1;
        V(2, 2) = 0;

        V(3, 0) = 1;
        V(3, 1) = 0;
        V(3, 2) = 0;

        V(4, 0) = 0;
        V(4, 1) = 0;
        V(4, 2) = 1;

        V(5, 0) = 1;
        V(5, 1) = 0;
        V(5, 2) = 1;

        V(6, 0) = 1;
        V(6, 1) = 1;
        V(6, 2) = 1;

        V(7, 0) = 0;
        V(7, 1) = 1;
        V(7, 2) = 1;

        F(0, 0) = 0;
        F(0, 1) = 1;
        F(0, 2) = 2;

        F(1, 0) = 0;
        F(1, 1) = 2;
        F(1, 2) = 3;

        F(2, 0) = 5;
        F(2, 1) = 6;
        F(2, 2) = 7;

        F(3, 0) = 5;
        F(3, 1) = 7;
        F(3, 2) = 4;

        F(4, 0) = 5;
        F(4, 1) = 3;
        F(4, 2) = 2;

        F(5, 0) = 5;
        F(5, 1) = 2;
        F(5, 2) = 6;

        F(6, 0) = 4;
        F(6, 1) = 7;
        F(6, 2) = 1;

        F(7, 0) = 4;
        F(7, 1) = 1;
        F(7, 2) = 0;

        F(8, 0) = 1;
        F(8, 1) = 7;
        F(8, 2) = 2;

        F(9, 0) = 2;
        F(9, 1) = 7;
        F(9, 2) = 6;

        F(10, 0) = 0;
        F(10, 1) = 3;
        F(10, 2) = 4;

        F(11, 0) = 4;
        F(11, 1) = 3;
        F(11, 2) = 5;
    }
}