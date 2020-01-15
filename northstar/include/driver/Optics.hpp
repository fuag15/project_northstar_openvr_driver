#pragma once

#include "driver/IOptics.hpp"
#include "math/Types.hpp"
#include "math/IVectorFactory.hpp"
#include "math/IMatrixFactory.hpp"
#include "math/IGeometry.hpp"
#include "math/ISpaceAdapter.hpp"
#include "math/IWorldAdapter.hpp"
#include "openvr_driver.h"
#include "driver/Settings.hpp"
#include "utility/ILogger.hpp"

#include <memory>

namespace northstar {
    namespace driver {
        class COptics : public IOptics {
        public:
            COptics(
                vr::IVRSettings* pVRSettings,
                std::shared_ptr<northstar::math::IWorldAdapter> pWorldAdapter,
                std::shared_ptr<northstar::math::ISpaceAdapter> pSpaceAdapter,
                std::shared_ptr<northstar::math::IGeometry> pGeometry,
                std::shared_ptr<northstar::math::IMatrixFactory> pMatrixFactory,
                std::shared_ptr<northstar::math::IVectorFactory> pVectorFactory,
                std::shared_ptr<northstar::utility::ILogger> pLogger);

            virtual northstar::math::types::Vector2d EyeUVToScreenUV(const vr::EVREye& eEye, const northstar::math::types::Vector2d& v2dEyeUV) override final;
            virtual northstar::math::types::Vector4d GetEyeProjectionLRTB(const vr::EVREye& eEye) override final;
        private:
            static constexpr size_t x_nNumberOfSolverIterations = 7;
            static constexpr std::array<double, 2> x_daInitialWarpGuess = { 0.5, 0.5 };
            static constexpr std::array<double, 2> x_daIterativeSolverErrorResult = { 0.0, 0.0 };
            static constexpr std::array<double, 3> x_daLensProxySphereOrigin = { 0.0, 0.0, 0.0 };
            static constexpr double x_dLensProxySphereRadius = 0.5;
            static constexpr double x_dIterativeSolverGradiantEpsilon = 0.0001;
            static constexpr double x_dIterativeSolverStepWeight = 1.0 / 7.0;

            struct SEllipsisAxis {
                double dMinor;
                double dMajor;
            };

            struct SEyeConfiguration {
                SEllipsisAxis sEllipsisAxis = { 0 };
                northstar::math::types::Vector3d v3dScreenForward;
                northstar::math::types::Vector3d v3dScreenPosition;
                northstar::math::types::Vector3d v3dEyePosition;
                northstar::math::types::Quaterniond qdEyeRotation;
                northstar::math::types::Vector4d v4dCameraProjectionFrustumExtentsLRTB;
                northstar::math::types::AffineMatrix4d m4dSphereToWorldSpace;
                northstar::math::types::AffineMatrix4d m4dWorldToScreenSpace;
                northstar::math::types::AffineMatrix4d m4dWorldToSphereSpace;
                northstar::math::types::ProjMatrix4d m4dCameraProjection;
                northstar::math::types::ProjMatrix4d m4dClipToWorld;
            };

            typedef std::unordered_map<
                northstar::math::types::Vector2d,
                northstar::math::types::Vector2d,
                northstar::math::types::SHasher<northstar::math::types::Vector2d>> UVWarpMap;

            SEyeConfiguration LoadConfigFromEye(const vr::EVREye& eEye);
            northstar::math::types::Vector2d ReverseProjectEyeUVToDisplayUV(const vr::EVREye& eEye, const northstar::math::types::Vector2d& v2dTargetEyeUV);
            northstar::math::types::Vector2d IterativeGradientUVWarpSolve(const vr::EVREye& eEye, const northstar::math::types::Vector2d& v2dEyeUV, const northstar::math::types::Vector2d& v2dWarpUVGuess);

            vr::IVRSettings* m_pVRSettings;
            std::shared_ptr<northstar::math::IVectorFactory> m_pVectorFactory;
            std::shared_ptr<northstar::math::IMatrixFactory> m_pMatrixFactory;
            std::shared_ptr<northstar::math::IGeometry> m_pGeometry;
            std::shared_ptr<northstar::math::IWorldAdapter> m_pWorldAdapter;
            std::shared_ptr<northstar::math::ISpaceAdapter> m_pSpaceAdapter;
            std::shared_ptr<northstar::utility::ILogger> m_pLogger;
            std::unordered_map<vr::EVREye, SEyeConfiguration> m_umEyeConfigs;
            std::unordered_map<vr::EVREye, UVWarpMap> m_umUVWarps;
        };
    }
}
