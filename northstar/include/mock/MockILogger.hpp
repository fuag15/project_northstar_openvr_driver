#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "utility/ILogger.hpp"

namespace northstar {
    namespace utility {
        class MockILogger : public ILogger
        {
        public:
            MAKE_MOCK1(Log, void(const char* pchMessage), override);
            MAKE_MOCK1(LogDebug, void(const char* pchMessage), override);
        };
    }
}
