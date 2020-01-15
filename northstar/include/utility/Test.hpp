#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>
#include <ST/XRTypes.h>
#include <openvr_driver.h>

#include "math/Types.hpp"

namespace northstar {
    namespace test {
        namespace utils {
            template <class T, class R>
            void CompareQuaternion(const T& a, const R& b) {
                CHECK(a.w == b.w);
                CHECK(a.x == b.x);
                CHECK(a.y == b.y);
                CHECK(a.z == b.z);
            }

            template <class T, class R>
            void CompareIndexed(const T& a, const R& b, size_t unStart, size_t unEnd) {
                for (size_t i = unStart; i <= unEnd; i++)
                    CHECK(a[i] == b[i]);
            }

            template <class T, class R>
            void CompareEigenMatrix(const T& a, const R& b, size_t rows, size_t columns) {
                for (size_t r = 0; r < rows; r++)
                    for (size_t c = 0; c < columns; c++)
                        CHECK(a(r, c) == b(r, c));
            }

            ST::XRPose DefaultValidPoseFromStructureSensor(const std::array<float, 16>& ReferenceRowMajorPositionTransform);
            vr::DriverPose_t GenerateExpectedOpenVRPoseGivenStructureSensorPose(
                const ST::XRPose& StructureSensorPose,
                const std::array<float, 16>& ReferenceRowMajorPositionTransform,
                double dReferenceCurrentUnixTimestamp);

            northstar::math::types::AffineMatrix4d CreateMatrixFromArray(const std::array<double, 16>& adData);
        }
    }
}