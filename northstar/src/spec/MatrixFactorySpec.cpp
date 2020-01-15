#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/MatrixFactory.hpp"
#include "utility/Test.hpp"

using namespace northstar::test::utils;
using namespace northstar::math::types;

TEST_SUITE("MatrixFactory") {
    SCENARIO("When creating a matrix from a array") {
        auto Subject = northstar::math::CMatrixFactory();
        GIVEN("The array is row major") {
            WHEN("Given an array of doubles") {
                auto rowMajorArray = std::array<double, 16>{
                    1, 2, 3, 4,
                    5, 6, 7, 8,
                    9, 10, 11, 12,
                    13, 14, 15, 16 };

                AffineMatrix4d expected;
                expected.matrix() <<
                    1, 2, 3, 4,
                    5, 6, 7, 8,
                    9, 10, 11, 12,
                    13, 14, 15, 16;

                THEN("It creates the appropriate transform") {
                    auto received = Subject.FromRowMajorArray(rowMajorArray);
                    CompareEigenMatrix(received, expected, 4, 4);
                }
            }
        }
    }

    SCENARIO("When creating translations") {
        auto Subject = northstar::math::CMatrixFactory();
        WHEN("Given a translation vector") {
            auto givenCoeffs = Vector3d(1, 2, 3);
            auto expectedTranslation = Translation3d(1, 2, 3);
            THEN("It returns the expected translation") {
                auto result = Subject.TranslationFromVector3d(givenCoeffs);
                CHECK(expectedTranslation.x() == result.x());
                CHECK(expectedTranslation.y() == result.y());
                CHECK(expectedTranslation.z() == result.z());
            }
        }
    }

    //TODO: Test this behaviorally
    SCENARIO("When creating a OpenGL Projection from Frustum extents") {
        auto Subject = northstar::math::CMatrixFactory();
        WHEN("Given extents") {
            THEN("It creates an appropriate projection") {}
        }
    }
}