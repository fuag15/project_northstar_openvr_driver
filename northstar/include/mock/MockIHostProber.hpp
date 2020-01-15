#pragma once

#include "doctest/doctest.h"
#include "doctest/trompeloeil.hpp"

#include "utility/IHostProber.hpp"

#include <optional>

namespace northstar {
    namespace utility {
        class MockIHostProber : public IHostProber {
        public:
            MAKE_CONST_MOCK0(ProbeDisplayOriginX, std::optional<long>(), override);
        };
    }
}
