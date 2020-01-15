#pragma once

#include <optional>
#include <openvr_driver.h>
#include <LeapC.h>

#include "driver/Types.hpp"

namespace northstar {
    namespace driver {
        class ISensorFrameCoordinator {
        public:
            virtual std::optional<vr::DriverPose_t> GetOpenVRHeadPose() = 0;
            virtual std::optional<LEAP_HAND> GetLeapHandPose(const northstar::driver::types::EHand& eHand) = 0;
            virtual void ResetFrameState() = 0;
            virtual void SubmitOpenVRHeadsetPose(const vr::DriverPose_t& sDriverPose) = 0;
        };
    }
}