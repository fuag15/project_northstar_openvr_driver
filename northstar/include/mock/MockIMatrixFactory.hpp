#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/IMatrixFactory.hpp"

namespace northstar {
    namespace math {
        class MockIMatrixFactory: public IMatrixFactory
        {
        public:
            MAKE_CONST_MOCK1(FromRowMajorArray, types::AffineMatrix4d(const std::array<double, 16>&), override);
            MAKE_CONST_MOCK1(TranslationFromVector3d, types::Translation3d(const types::Vector3d&), override);
            MAKE_CONST_MOCK2(FromTraslationAndRotation, types::AffineMatrix4d(const types::Vector3d& v3dTranslation, const types::Quaterniond& qdRotation), override);
            MAKE_CONST_MOCK4(OpenGLProjectionFromFrustumExtents, types::ProjMatrix4d(const double&, const double&, const double&, const double&), override);
        };
    }
}
