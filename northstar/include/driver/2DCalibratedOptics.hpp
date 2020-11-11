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
        class C2DCalibratedOptics : public IOptics {
        public:
            C2DCalibratedOptics(
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
            struct SEyeConfiguration {
                float baseline = 0;
                std::array<float, 16> UVToRectilinearXAxisCoefficients = { 0 };
                std::array<float, 16> UVToRectilinearYAxisCoefficients = { 0 };
                northstar::math::types::ProjMatrix4d m4dCameraProjection;
                northstar::math::types::Vector4d v4dCameraProjectionFrustumExtentsLRTB;
            };

            typedef std::unordered_map<
                northstar::math::types::Vector2d,
                northstar::math::types::Vector2d,
                northstar::math::types::SHasher<northstar::math::types::Vector2d>> UVWarpMap;

            SEyeConfiguration LoadConfigFromEye(const vr::EVREye& eEye);

            // TODO: better name for this
            northstar::math::types::Vector2d PolynomialWarpSolve(const vr::EVREye& eEye, const northstar::math::types::Vector2d& v2dTargetEyeUV) const noexcept;

            // TODO: better name for this and put it in geometry?
            float EvaluateThirdDegree2DPolynomial(float XPosition, float YPosition, const std::array<float, 16>& Coefficients) const noexcept;

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
