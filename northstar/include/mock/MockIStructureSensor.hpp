#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include <ST/XRSession.h>
#include "driver/IStructureSensor.hpp"

namespace northstar {
    namespace driver {
        class MockIStructureSensor : 
            public IStructureSensor {
            public:
                MAKE_MOCK2(GetPose, bool(ST::XRPose&, EPoseRetrievalError&), override);
                MAKE_MOCK0(SessionStartWasSuccessful, bool(), override);
                MAKE_MOCK0(PollingUpdateLoop, void(), override);
        };
    }
}