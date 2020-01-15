#pragma once

#include <optional>

namespace northstar {
    namespace driver {
        class ILeapMotion {
        public:
            virtual std::optional<LEAP_TRACKING_EVENT> GetFrame() = 0;
            virtual std::optional<LEAP_TRACKING_EVENT> GetInterpolatedFrame() = 0;
            virtual bool IsConnected() const = 0;
        };
    }
}