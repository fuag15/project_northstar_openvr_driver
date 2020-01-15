#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/SpaceAdapter.hpp"
#include "math/IVectorFactory.hpp"
#include "mock/MockIVectorFactory.hpp"
#include "math/Types.hpp"
#include "utility/Test.hpp"
#include "math/SpaceAdapter.hpp"

using namespace northstar::test::utils;
using northstar::math::MockIVectorFactory;
using northstar::math::IVectorFactory;
using northstar::math::types::Vector2d;
using northstar::math::types::Vector3d;
using trompeloeil::_;

TEST_SUITE("SpaceAdapter") {
    SCENARIO("When converting from Unity UV to UnityClipForwardZ") {
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CSpaceAdapter(pMockVectorFactory);
        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_))
            .RETURN(Vector3d(_1[0], _1[1], _1[2]));

        ALLOW_CALL(*pMockVectorFactory, V2DFromArray(_))
            .RETURN(Vector2d(_1[0], _1[1]));

        Vector2d v2dUV;
        Vector3d v3dExpected;

        WHEN("Given a UV in the upper left corner") {
            v2dUV << 0, 1;
            v3dExpected << -1, 1, 1;
            auto v3dReceived = Subject.FromUnityUVToUnityClipUnitForwardZ(v2dUV);
            THEN("it returns an upper left corner clip point") {
                CompareIndexed(v3dReceived, v3dExpected, 0, 2);
            }
        }

        WHEN("Given a UV in the lower right corner") {
            v2dUV << 1, 0;
            v3dExpected << 1, -1, 1;
            auto v3dReceived = Subject.FromUnityUVToUnityClipUnitForwardZ(v2dUV);
            THEN("it returns an lower right corner clip point") {
                CompareIndexed(v3dReceived, v3dExpected, 0, 2);
            }
        }

        WHEN("Given a UV in the center") {
            v2dUV << 0.5, 0.5;
            v3dExpected << 0, 0, 1;
            auto v3dReceived = Subject.FromUnityUVToUnityClipUnitForwardZ(v2dUV);
            THEN("it returns a center clip point") {
                CompareIndexed(v3dReceived, v3dExpected, 0, 2);
            }
        }
    }

    // TODO: Test
    SCENARIO("When converting from UnityClipNearPlane to UnityUV") {
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CSpaceAdapter(pMockVectorFactory);
        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_))
            .RETURN(Vector3d(_1[0], _1[1], _1[2]));

        ALLOW_CALL(*pMockVectorFactory, V2DFromArray(_))
            .RETURN(Vector2d(_1[0], _1[1]));

        Vector3d v3dClipNearPlane;
        Vector2d v2dExpected;

        // TODO: Test
        WHEN("Given clip point in the upper left corner") {
            THEN("it returns an upper left corner UV") {}
        }

        // TODO: Test
        WHEN("Given a clip point in the lower right corner") {
            THEN("it returns an lower right corner UV") { }
        }

        // TODO: Test
        WHEN("Given a clip point in the center") {
            THEN("it returns a center UV") {}
        }
    }

    // TODO: Test this
    SCENARIO("When Projecting a point into another space") {
        WHEN("Given a point") {
            THEN("The correct projected point is returned") {}
        }
    }
}
