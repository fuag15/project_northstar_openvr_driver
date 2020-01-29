#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include <LeapC.h>
#include <openvr_driver.h>

#include "math/ISkeletalAdapter.hpp"

namespace northstar {
    namespace math {
        class MockISkeletalAdapter : public ISkeletalAdapter {
        public:
            MAKE_CONST_MOCK5(
                FromLeapMotionHandToOVRBonePoseArray, 
                void(
                    const vr::DriverPose_t&,
                    const types::AffineMatrix4d&,
                    const types::AffineMatrix4d&,
                    const LEAP_HAND&, 
                    std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>&), 
                override);
        };
    }
}
