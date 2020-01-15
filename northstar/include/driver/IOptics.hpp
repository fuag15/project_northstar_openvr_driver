#pragma once

#include <openvr_driver.h>

#include "math/Types.hpp"

namespace northstar {
    namespace driver {
        class IOptics {
        public:
            virtual northstar::math::types::Vector2d EyeUVToScreenUV(const vr::EVREye& eEye, const northstar::math::types::Vector2d& v2dTargetEyeUV) = 0;
            virtual northstar::math::types::Vector4d GetEyeProjectionLRTB(const vr::EVREye& eEye) = 0;
        };
    }
}
