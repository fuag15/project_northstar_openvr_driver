#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/VectorFactory.hpp"
#include "utility/Test.hpp"

using namespace northstar::test::utils;
using namespace northstar::math::types;

TEST_SUITE("VectorFactory") {
    SCENARIO("When creating a 3d vector from an array") {
        auto Subject = northstar::math::CVectorFactory();
        WHEN("Given an array of doubles") {
            auto GivenArray = std::array<double, 3>{
                1, 2, 3 };

            Vector3d expected(1, 2, 3);
            THEN("It creates the appropriate vector") {
                auto received = Subject.V3DFromArray(GivenArray);
                CompareIndexed(expected, received, 0, 2);
            }
        }
    }

    SCENARIO("When creating a 2d vector from an array") {
        auto Subject = northstar::math::CVectorFactory();
        WHEN("Given an array of doubles") {
            auto GivenArray = std::array<double, 2>{
                1, 2};

            Vector2d expected(1, 2);
            THEN("It creates the appropriate vector") {
                auto received = Subject.V2DFromArray(GivenArray);
                CompareIndexed(expected, received, 0, 1);
            }
        }
    }

    SCENARIO("When converting from a 3d to a 4d vector") {
        auto Subject = northstar::math::CVectorFactory();
        WHEN("Given a 3d vector and a w component") {
            auto v3dGivenXYZ = Vector3d(1, 2, 3);
            double dGivenW = 4.0;
            auto v4dExpected = Vector4d(1, 2, 3, dGivenW);
            THEN("It returns the right 4d vector") {
                auto received = Subject.V4DFromV3DXYZandW(v3dGivenXYZ, dGivenW);
                CompareIndexed(received, v4dExpected, 0, 3);
            }
        }

        WHEN("Given an 4d array representin xyzw") {
            std::array<double, 4> adGiven = { 1, 2, 3, 4 };
            THEN("it returns the right 4d vector") {
                auto received = Subject.V4DFromXYZWArray(adGiven);
                CompareIndexed(received, adGiven, 0, 3);
            }
        }
    }

    SCENARIO("When converting from a 4d to a 3d vector") {
        auto Subject = northstar::math::CVectorFactory();
        WHEN("Given a 4d vector") {
            auto v4dGiven = Vector4d(1, 2, 3, 4);
            auto v3dExpected = Vector3d(1, 2, 3);
            THEN("It returns a 3d vector of the XYZ components") {
                auto received = Subject.V3DXYZFromV4D(v4dGiven);
                CompareIndexed(received, v3dExpected, 0, 2);
            }
        }
    }

    SCENARIO("When creating a 3d ray") {
        auto Subject = northstar::math::CVectorFactory();
        WHEN("given an origin an normal array") {
            auto GivenOrigin = std::array<double, 3>{1, 2, 3};
            auto GivenDirection = std::array<double, 3>{0, 1, 0};
            
            Ray3d expected(
                Vector3d(GivenOrigin[0], GivenOrigin[1], GivenOrigin[2]),
                Vector3d(GivenDirection[0], GivenDirection[1], GivenDirection[2]));
            
            THEN("It creates the appropriate ray") {
                auto received = Subject.R3DFromOriginAndDirectionArray(GivenOrigin, GivenDirection);
                CompareIndexed(received.origin(), expected.origin(), 0, 2);
                CompareIndexed(received.direction(), expected.direction(), 0, 2);
            }
        }

        WHEN("given an origin an normal vector") {
            Vector3d GivenOrigin(0, 1, 2);
            Vector3d GivenDirection(0, 1, 0);
            Ray3d expected(GivenOrigin,GivenDirection);
            
            THEN("It creates the appropriate ray") {
                auto received = Subject.R3DFromOriginAndDirectionV3D(GivenOrigin, GivenDirection);
                CompareIndexed(received.origin(), expected.origin(), 0, 2);
                CompareIndexed(received.direction(), expected.direction(), 0, 2);
            }
        }
    }
}
