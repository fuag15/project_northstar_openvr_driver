#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>
#include <ST/XRTypes.h>

#include <utility>

#include "math/IWorldAdapter.hpp"

namespace northstar {
    namespace math {
        class MockIWorldAdapter : public IWorldAdapter
        {
            MAKE_MOCK1(FromStructureSensorPoseToOpenVRSpace, types::SPose(const ST::XRPose&), override);
            MAKE_MOCK1(FromStructureSensorLinearVectorArrayToOpenVRSpace, types::Vector3d(const std::array<double, 3>&), override);
            MAKE_MOCK1(FromStructureSensorAngularVectorArrayToOpenVRSpace, types::Vector3d(const std::array<double, 3>&), override);
            MAKE_MOCK2(ConversionMatrixFromLeapMotionTrackingSpaceToHMDRelativeSpace, types::AffineMatrix4d(const types::Vector3d&, const types::Quaterniond&), override);
            MAKE_MOCK1(ConversionMatrixFromHMDSpaceToOpenVRWorldSpace, types::AffineMatrix4d(const vr::DriverPose_t&), override);
            MAKE_MOCK2(FromLeapMotionVelocityToOpenVRVelocity, types::Vector3d(const types::AffineMatrix4d&, const types::Vector3d&), override);
            MAKE_MOCK1(FromUnityQuaternionToOpenVRQuaternion, types::Quaterniond(const types::Quaterniond&), override);
            MAKE_MOCK1(FromUnityPositionToOpenVRPosition, types::Vector3d(const types::Vector3d&), override);
            MAKE_MOCK1(FromUnityProjectionExtentsLRTBToOpenVRProjectionExtentsLRTB, types::Vector4d(const types::Vector4d&), override);
            MAKE_MOCK1(FromUnityMatrix4dToOpenVRMatrix4d, types::AffineMatrix4d(const types::AffineMatrix4d&), override);
            MAKE_MOCK1(FromOpenGLUVToUnityUV, types::Vector2d(const types::Vector2d&), override);
            MAKE_MOCK1(FromUnityUVToOpenGLUV, types::Vector2d(const types::Vector2d&), override);
        };
    }
}
