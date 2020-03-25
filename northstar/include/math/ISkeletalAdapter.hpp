#pragma once

#include <LeapC.h>
#include <openvr_driver.h>

#include "driver/Settings.hpp"
#include "driver/Types.hpp"
#include "math/Types.hpp"

namespace northstar {
    namespace math {
        class ISkeletalAdapter {
        public:
            virtual void FromLeapMotionHandToOVRBonePoseArray(
                const vr::DriverPose_t& sPose,
                const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
                const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
                const LEAP_HAND& sLeapHand, 
                std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>& saOpenVRBoneTargets,
                const northstar::driver::types::EHand& eHand) const = 0;
        };
    }
}