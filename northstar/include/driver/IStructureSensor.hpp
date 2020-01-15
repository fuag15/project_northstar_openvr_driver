#pragma once

#include <ST/XRSession.h>

namespace northstar {
    namespace driver {
        class IStructureSensor
        {
        public:
            enum class EPoseRetrievalError {
                None,
                SessionStartFailed,
                SessionNotRunning,
                SessionInitializing,
                InvalidPoseRecieved
            };

            virtual bool GetPose(ST::XRPose& pose, EPoseRetrievalError& error) = 0;
            virtual bool SessionStartWasSuccessful() = 0;

            virtual void PollingUpdateLoop() = 0;
        };
    }
}
