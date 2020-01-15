#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "utility/ITimeProvider.hpp"

namespace northstar {
    namespace utility {
        class MockITimeProvider : public ITimeProvider {
            MAKE_CONST_MOCK0(CurrentUnixTimestamp, double());
        };
    }
}
