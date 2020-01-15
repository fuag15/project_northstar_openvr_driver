#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>
#include <LeapC.h>

#include "driver/ILeapMotion.hpp"

namespace northstar {
    namespace driver {
        class MockILeapMotion : public ILeapMotion {
        public:
            MAKE_MOCK0(GetFrame, std::optional<LEAP_TRACKING_EVENT>(), override);
            MAKE_MOCK0(GetInterpolatedFrame, std::optional<LEAP_TRACKING_EVENT>(), override);
            MAKE_CONST_MOCK0(IsConnected, bool(), override);
        };
    }
}