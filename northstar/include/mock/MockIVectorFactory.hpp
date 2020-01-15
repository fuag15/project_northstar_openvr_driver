#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/IVectorFactory.hpp"

namespace northstar {
    namespace math {
        class MockIVectorFactory: public IVectorFactory
        {
        public:
            MAKE_CONST_MOCK1(V3DFromArray, types::Vector3d(const std::array<double, 3>&), override);
            MAKE_CONST_MOCK1(V2DFromArray, types::Vector2d(const std::array<double, 2>&), override);
            MAKE_CONST_MOCK2(V4DFromV3DXYZandW, types::Vector4d(const types::Vector3d&, const double&), override);
            MAKE_CONST_MOCK1(V3DXYZFromV4D, types::Vector3d(const types::Vector4d&), override);
            MAKE_CONST_MOCK1(V4DFromXYZWArray, types::Vector4d(const std::array<double, 4>&), override);
            MAKE_CONST_MOCK2(R3DFromOriginAndDirectionArray, types::Ray3d(const std::array<double, 3>&, const std::array<double, 3>&), override);
            MAKE_CONST_MOCK2(R3DFromOriginAndDirectionV3D, types::Ray3d(const types::Vector3d&, const types::Vector3d&), override);
        };
    }
}
