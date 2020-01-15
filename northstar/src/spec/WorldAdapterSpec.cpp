#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>
#include <ST/XRTypes.h>
#include <Eigen/Dense>

#include "mock/MockIMatrixFactory.hpp"
#include "mock/MockIVectorFactory.hpp"

#include "math/WorldAdapter.hpp"
#include "math/IMatrixFactory.hpp"
#include "math/IVectorFactory.hpp"
#include "math/Types.hpp"

#include "utility/Test.hpp"
#include <array>

using trompeloeil::_;
using northstar::math::MockIMatrixFactory;
using northstar::math::IMatrixFactory;
using northstar::math::MockIVectorFactory;
using northstar::math::IVectorFactory;
using northstar::math::types::AffineMatrix4d;
using northstar::math::types::Vector3d;

using namespace northstar::test::utils;

TEST_SUITE("WorldAdapter") {
    SCENARIO("When converting to OpenVR from a StrucureSensor Pose") {
        auto pMockMatrixFactory = std::make_shared<MockIMatrixFactory>();
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CWorldAdapter(
            pMockMatrixFactory,
            pMockVectorFactory);

        ALLOW_CALL(*pMockMatrixFactory, FromRowMajorArray(_))
            .RETURN(CreateMatrixFromArray(_1));

        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_))
            .RETURN(Vector3d(_1[0], _1[1], _1[2]));

        // TODO: test
        WHEN("Given a pose") {
            THEN("it returns the appropriate open vr space matrix") {}
        }
    }

    SCENARIO("World adapter should convert vectors from structure sensor space to OPenVR Space") {
        auto pMockMatrixFactory = std::make_shared<MockIMatrixFactory>();
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CWorldAdapter(
            pMockMatrixFactory,
            pMockVectorFactory);

        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_))
            .RETURN(Vector3d(_1[0], _1[1], _1[2]));

        // TODO: test
        WHEN("Given a linear vector in structure sensor space") {
            std::array<double, 3> adStructureVector{ 1, 2, 3 };
            std::array<double, 3> adExpected{ -1, -2, -3 };
            THEN("It returns the apropriate open vr space vector") {
                auto Result = Subject.FromStructureSensorLinearVectorArrayToOpenVRSpace(adStructureVector);
                CompareIndexed(Result, adExpected, 0, 2);
            }
        }

        // TODO: test
        WHEN("Given an angular vector in structure sensor space") {
            std::array<double, 3> adStructureVector{ 1, 2, 3 };
            std::array<double, 3> adExpected{ -1, -2, -3 };
            THEN("It returns the apropriate open vr space vector") {
                auto Result = Subject.FromStructureSensorAngularVectorArrayToOpenVRSpace(adStructureVector);
                CompareIndexed(Result, adExpected, 0, 2);
            }
        }
    }

    SCENARIO("World adapter should support converting from LeapMotion space to OVR space") {
        auto pMockMatrixFactory = std::make_shared<MockIMatrixFactory>();
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CWorldAdapter(
            pMockMatrixFactory,
            pMockVectorFactory);

        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_))
            .RETURN(Vector3d(_1[0], _1[1], _1[2]));

        // TODO: Test
        WHEN("asked to generate a conversion matrix given a config") {
            THEN("the conversion matrix is as expected") {

            }
        }

        // TODO: Test
        WHEN("Asked to use a conversion matrix to convert velocity in leap space to hmp space") {
            THEN("the conversion is as expected") {

            }
        }

        // TODO: Test
        WHEN("Asked to use a conversion matrix to convert drive pose to hmd space") {
            THEN("the conversion matrix is as expected") {

            }
        }
    }

    // TOOD: Test
    SCENARIO("World adapter should support converting from UnitySpace (used for configuration) to OpenVR space") {
        auto pMockMatrixFactory = std::make_shared<MockIMatrixFactory>();
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CWorldAdapter(
            pMockMatrixFactory,
            pMockVectorFactory);

        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_))
            .RETURN(Vector3d(_1[0], _1[1], _1[2]));

        WHEN("Converting a Unity Pose to an OpenVR Pose") {
            THEN("THe conversion is as expected") {}
        }

        WHEN("Converting a Unity Quaternion to HMD Quaternion") {
            THEN("The conversion quatertion is as expected") {}
        }

        WHEN("Converting a Unity Position to HMD Position") {
            THEN("The conversion position is as expected") {}
        }

        WHEN("Converting a Unity LRTB Projection Extent") {
            THEN("The conversion extents are as expected") {}
        }
    }

    // TOOD: Test
    SCENARIO("World adapter should support converting between Unity space UV and OpenVR space UV") {
        auto pMockMatrixFactory = std::make_shared<MockIMatrixFactory>();
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CWorldAdapter(
            pMockMatrixFactory,
            pMockVectorFactory);

        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_))
            .RETURN(Vector3d(_1[0], _1[1], _1[2]));

        WHEN("Converting a Unity UV to an OpenVR UV") {
            THEN("THe conversion is as expected") {}
        }

        WHEN("Converting an OpenVR UV to a Unity UV") {
            THEN("THe conversion is as expected") {}
        }
    }
}
