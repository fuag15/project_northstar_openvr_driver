#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include <openvr_driver.h>
#include "math/Types.hpp"
#include "driver/IOptics.hpp"

namespace northstar {
    namespace driver {
        class MockIOptics : public IOptics{
            MAKE_MOCK2(EyeUVToScreenUV, northstar::math::types::Vector2d(const vr::EVREye&, const northstar::math::types::Vector2d&), override);
            MAKE_MOCK1(GetEyeProjectionLRTB, northstar::math::types::Vector4d(const vr::EVREye&), override);
        };
    }
}
