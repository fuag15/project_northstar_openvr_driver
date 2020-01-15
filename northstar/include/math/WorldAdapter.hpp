#pragma once

#include <memory>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#define _USE_MATH_DEFINES
#include <cmath>

#include "math/IWorldAdapter.hpp"
#include "math/IMatrixFactory.hpp"
#include "math/IVectorFactory.hpp"

namespace northstar {
    namespace math {
        class CWorldAdapter : public IWorldAdapter {
        public:
            CWorldAdapter(
                std::shared_ptr<IMatrixFactory> pMatrixFactory,
                std::shared_ptr<IVectorFactory> pVectorFactory);

            virtual types::SPose FromStructureSensorPoseToOpenVRSpace(const ST::XRPose& Pose) override final;
            virtual types::Vector3d FromStructureSensorLinearVectorArrayToOpenVRSpace(const std::array<double, 3>& Vector) override final;
            virtual types::Vector3d FromStructureSensorAngularVectorArrayToOpenVRSpace(const std::array<double, 3>& Vector) override final;
            virtual types::AffineMatrix4d ConversionMatrixFromLeapMotionTrackingSpaceToHMDRelativeSpace(const types::Vector3d& v3dHeadRelativeLeapPosition, const types::Quaterniond& qdHeadRelativeLeapOrientation) override final;
            virtual types::AffineMatrix4d ConversionMatrixFromHMDSpaceToOpenVRWorldSpace(const vr::DriverPose_t& sOVRPose) override final;
            virtual types::Vector3d FromLeapMotionVelocityToOpenVRVelocity(const types::AffineMatrix4d& m4dConversionMatrix, const types::Vector3d& v3dLeapVelocity) override final;
            virtual types::Quaterniond FromUnityQuaternionToOpenVRQuaternion(const types::Quaterniond& qdUnityRotation) override final;
            virtual types::Vector3d FromUnityPositionToOpenVRPosition(const types::Vector3d& v3dUnityPosition) override final;
            virtual types::Vector4d FromUnityProjectionExtentsLRTBToOpenVRProjectionExtentsLRTB(const types::Vector4d& v4dUnityProjectionExtentsLRTB) override final;
            virtual types::AffineMatrix4d FromUnityMatrix4dToOpenVRMatrix4d(const types::AffineMatrix4d& v4dUnityPose) override final;
            virtual types::Vector2d FromOpenGLUVToUnityUV(const types::Vector2d& v2dOpenGLUV) override final;
            virtual types::Vector2d FromUnityUVToOpenGLUV(const types::Vector2d& v2dUnityUV) override final;

        private:
            static constexpr double x_dMillimetersToMeters = 0.001;
            std::shared_ptr<IMatrixFactory> m_pMatrixFactory;
            std::shared_ptr<IVectorFactory> m_pVectorFactory;
        };
    }
}