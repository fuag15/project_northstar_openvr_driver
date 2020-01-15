// World Reference Frame Notes

// Structure:
// left handed
// +y is up
// -x is right
// -z is forward
// it is using column major storage
// distance unit is meters

// OpenVR:
// right handed
// +y is up
// +x is right
// -z is forward
// row major storage
// distance unit is meters
#pragma once

#include <utility>

#include <Eigen/Dense>
#include <ST/XRTypes.h>
#include <openvr_driver.h>

#include "math/Types.hpp"

namespace northstar {
    namespace math {
        class IWorldAdapter {
        public:
            virtual types::SPose FromStructureSensorPoseToOpenVRSpace(const ST::XRPose& Pose) = 0;
            virtual types::Vector3d FromStructureSensorLinearVectorArrayToOpenVRSpace(const std::array<double, 3>& Vector) = 0;
            virtual types::Vector3d FromStructureSensorAngularVectorArrayToOpenVRSpace(const std::array<double, 3>& Vector) = 0;
            virtual types::AffineMatrix4d ConversionMatrixFromLeapMotionTrackingSpaceToHMDRelativeSpace(const types::Vector3d& v3dHeadRelativeLeapPosition, const types::Quaterniond& qdHeadRelativeLeapOrientation) = 0;
            virtual types::AffineMatrix4d ConversionMatrixFromHMDSpaceToOpenVRWorldSpace(const vr::DriverPose_t& sOVRPose) = 0;
            virtual types::Vector3d FromLeapMotionVelocityToOpenVRVelocity(const types::AffineMatrix4d& m4dConversionMatrix, const types::Vector3d& v3dLeapVelocity) = 0;
            virtual types::Quaterniond FromUnityQuaternionToOpenVRQuaternion(const types::Quaterniond& qdUnityRotation) = 0;
            virtual types::Vector3d FromUnityPositionToOpenVRPosition(const types::Vector3d& v3dUnityPosition) = 0;
            virtual types::Vector4d FromUnityProjectionExtentsLRTBToOpenVRProjectionExtentsLRTB(const types::Vector4d& v4dUnityProjectionExtentsLRTB) = 0;
            virtual types::AffineMatrix4d FromUnityMatrix4dToOpenVRMatrix4d(const types::AffineMatrix4d& v4dUnityPose) = 0;
            virtual types::Vector2d FromOpenGLUVToUnityUV(const types::Vector2d& v2dOpenGLUV) = 0;
            virtual types::Vector2d FromUnityUVToOpenGLUV(const types::Vector2d& v2dUnityUV) = 0;
        };
    }
}

