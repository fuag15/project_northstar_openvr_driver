#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "driver/ISensorFrameCoordinator.hpp"
#include "math/Types.hpp"

namespace northstar {
    namespace driver {
        class MockISensorFrameCoordinator : public ISensorFrameCoordinator {
        public:
            MAKE_MOCK0(GetOpenVRHeadPose, std::optional<vr::DriverPose_t>(), override);
            MAKE_MOCK1(GetLeapHandPose, std::optional<LEAP_HAND>(const northstar::driver::types::EHand&), override);
            MAKE_MOCK0(ResetFrameState, void(), override);
            MAKE_MOCK1(SubmitOpenVRHeadsetPose, void(const vr::DriverPose_t&), override);
        };
    }
}