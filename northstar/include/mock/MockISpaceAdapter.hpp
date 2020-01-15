#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/ISpaceAdapter.hpp"

namespace northstar {
    namespace math {
        class MockISpaceAdapter : public ISpaceAdapter {
            MAKE_MOCK1(FromUnityUVToUnityClipUnitForwardZ, types::Vector3d(const types::Vector2d&), override);
            MAKE_MOCK1(FromUnityClipNearPlaneToUnityUV, types::Vector2d(const types::Vector3d&), override);
            MAKE_MOCK2(ProjectPoint, types::Vector3d(const types::ProjMatrix4d&, const types::Vector3d&), override);
        };
    }
}
