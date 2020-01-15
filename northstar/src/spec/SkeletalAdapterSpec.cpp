#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/SkeletalAdapter.hpp"

TEST_SUITE("CSkeletalAdapter") {
    // TODO: test this
    SCENARIO("Must adapt from leap hands to OpenVRbone arrays") {
        WHEN("Given valid conversion matricies and data") {
            THEN("It should convert to appropriate bone positions") {}
        }
    }

}
