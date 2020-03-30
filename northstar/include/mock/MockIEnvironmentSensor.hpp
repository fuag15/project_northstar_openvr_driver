#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include <ST/XRSession.h>
#include <openvr_driver.h>

#include "driver/IEnvironmentSensor.hpp"

namespace northstar {
    namespace driver {
        class MockIEnvironmentSensor: 
            public IEnvironmentSensor{
            public:
                MAKE_MOCK2(GetPose, bool(vr::DriverPose_t&, EPoseRetrievalError&), override);
                MAKE_MOCK0(SessionStartWasSuccessful, bool(), override);
        };
    }
}