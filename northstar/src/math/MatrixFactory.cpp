#include "math/MatrixFactory.hpp"

using namespace northstar::math::types;

AffineMatrix4d northstar::math::CMatrixFactory::FromRowMajorArray(const std::array<double, 16>& pdArray) const {
    AffineMatrix4d m4d;
    m4d.matrix() <<
        pdArray[0], pdArray[1], pdArray[2], pdArray[3],
        pdArray[4], pdArray[5], pdArray[6], pdArray[7],
        pdArray[8], pdArray[9], pdArray[10], pdArray[11],
        pdArray[12], pdArray[13], pdArray[14], pdArray[15];

    return m4d;
}

Translation3d northstar::math::CMatrixFactory::TranslationFromVector3d(const types::Vector3d& v3dTranslation) const {
    return Translation3d(v3dTranslation);
}

/*
    this is written for OpenGL NDC space

    You can derive the math for this function by using Matrix Vector Multiplication,
    Systems of Equations, Similar Triangles, Trig, and knowing that the properties
    that must hold for this matrix are.

    from near to far maps to -1, 1
    left to right maps to -1, 1
    bottom to top maps to -1, 1
    rely on an implicit w divide to get depth in a reconstructable way

    TODO: derive here for doc
*/
ProjMatrix4d northstar::math::CMatrixFactory::OpenGLProjectionFromFrustumExtents(
    const double& dLeft,
    const double& dRight,
    const double& dTop,
    const double& dBottom) const {
    ProjMatrix4d m4d;
    m4d.matrix() <<
        2.0 * (1.0 / (dRight - dLeft)), 0, (dRight + dLeft) * (1.0 / (dRight - dLeft)), 0,
        0, 2.0 * (1.0 / (dBottom - dTop)), (dBottom + dTop) * (1.0 / (dBottom - dTop)), 0,
        0, 0, -(s_dZFar + s_dZNear) / (s_dZFar - s_dZNear), -(2.0 * s_dZFar * s_dZNear) / (s_dZFar - s_dZNear),
        0, 0, -1, 0;

    return m4d;
}
