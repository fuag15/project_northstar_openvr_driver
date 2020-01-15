#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/Types.hpp"

#include "math/IGeometry.hpp"

namespace northstar {
    namespace math {
        class MockIGeometry : public IGeometry {
            MAKE_CONST_MOCK2(Reflect, std::optional<types::Vector3d>(const types::Vector3d&, const types::Vector3d&), override);
            MAKE_CONST_MOCK2(IntersectWithPlane, std::optional<types::Vector3d>(const types::Ray3d&, const types::Plane3d&), override);
            MAKE_CONST_MOCK2(IntersectWithSphereFromWithin, std::optional<types::Vector3d>(const types::Ray3d&, const types::SSphere&), override);
        };
    }
}
