#pragma once

#include <optional>

namespace northstar {
    namespace utility {
        class IHostProber {
        public:
            virtual std::optional<long> ProbeDisplayOriginX() const = 0;
        };
    }
}