#pragma once

#include <openvr_driver.h>

namespace northstar {
    namespace driver {
        class IEnvironmentSensor
        {
        public:
            enum class EPoseRetrievalError {
                None,
                SessionStartFailed,
                SessionNotRunning,
                SessionInitializing,
                InvalidPoseRecieved
            };

            virtual bool GetPose(vr::DriverPose_t& pose, EPoseRetrievalError& error) = 0;
            virtual bool SessionStartWasSuccessful() = 0;
        };
    }
}
