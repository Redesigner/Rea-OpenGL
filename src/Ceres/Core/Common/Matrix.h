#pragma once

namespace Ceres
{
    struct Matrix
    {
        public:
            Matrix(
                float m11, float m12, float m13, float m14,
                float m21, float m22, float m23, float m24,
                float m31, float m32, float m33, float m34,
                float m41, float m42, float m43, float m44);

            static Matrix Identity();
            static Matrix Zero();

            Matrix operator*(const Matrix& b);

            float M[4][4];
    };
}