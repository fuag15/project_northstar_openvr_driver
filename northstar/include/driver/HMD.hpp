#include <openvr_driver.h>
#include <string>
#include <string_view>

#include "driver/IEnvironmentSensor.hpp"
#include "driver/IOptics.hpp"
#include "driver/Settings.hpp"
#include "driver/ISensorFrameCoordinator.hpp"
#include "openvr/IVRProperties.hpp"
#include "utility/Logger.hpp"
#include "utility/IHostProber.hpp"
#include "math/IVectorFactory.hpp"
#include "math/Types.hpp"


namespace northstar {
    namespace driver {
        class CHMD : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent
        {
        public:
            CHMD(vr::IVRSettings* pVRSettings,
                vr::IVRServerDriverHost* pVRServerDriverHost,
                std::shared_ptr<northstar::utility::IHostProber> pHostProber,
                std::shared_ptr<northstar::openvr::IVRProperties> pVRProperties,
                std::shared_ptr<northstar::driver::IEnvironmentSensor> pEnvironmentSensor,
                std::shared_ptr<northstar::math::IVectorFactory> pVectorFactory,
                std::shared_ptr<northstar::driver::IOptics> pOptics,
                std::shared_ptr<northstar::driver::ISensorFrameCoordinator> pSensorFrameCoordinator,
                std::shared_ptr<northstar::utility::ILogger> pLogger);

            virtual vr::EVRInitError Activate(vr::TrackedDeviceIndex_t unObjectId) override final;
            virtual void Deactivate() override final;
            virtual void EnterStandby() override final;
            virtual void* GetComponent(const char* pchComponentNameAndVersion) override final;
            virtual bool IsDisplayOnDesktop() override final;
            virtual bool IsDisplayRealDisplay() override final;
            virtual vr::DriverPose_t GetPose() override final;
            virtual void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override final;
            virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) override final;
            virtual void GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override final;
            virtual void GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom) override final;
            virtual vr::DistortionCoordinates_t ComputeDistortion(vr::EVREye eEye, float fU, float fV) override final;
            virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override final;

            void RunFrame();
            const std::string_view& GetSerialNumber() const;

        private:
            static constexpr int32_t x_iFallbackWindowOriginX = 0;
            static constexpr float x_fUserHeadToEyeDepthInMeters = 0.0f; // TODO: validate this
            static constexpr bool x_bDirectModeEnabled = false;
            static constexpr bool x_bIsDisplayOnDesktop = true;
            static constexpr std::string_view x_svSerialNumber = "1024";
            static constexpr std::string_view x_svModelNumber = "0";

            struct SDisplayConfiguration {
                northstar::math::types::Vector2i v2iWindowOrigin;
                northstar::math::types::Vector2i v2iWindowDimensions;
                northstar::math::types::Vector2i v2iEyeRenderAreaDimensions;
                double dFrequency;
                double dPhotonLatency;
            };

            struct SConfiguration{
                SDisplayConfiguration sDisplayConfiguration;
                bool bUseFakeScreenConfig;
                bool bUseFakeProjection;
                bool bUseFakeWarp;
                bool bUseFakeTracking;
                double dIPD;
            };

            struct SOpenVRState {
                vr::TrackedDeviceIndex_t unObjectId;
                vr::PropertyContainerHandle_t ulPropertyContainer;
            };

            void LoadConfiguration();
            void SetOpenVRConfiguration();
            void SetOpenVRProperties();

            SConfiguration m_sConfiguration;
            SOpenVRState m_sOpenVRState;

            std::shared_ptr<IEnvironmentSensor> m_pEnvironmentSensor;
            std::shared_ptr<northstar::math::IVectorFactory> m_pVectorFactory;
            std::shared_ptr<northstar::driver::IOptics> m_pOptics;
            std::shared_ptr<northstar::driver::ISensorFrameCoordinator> m_pSensorFrameCoordinator;
            std::shared_ptr<northstar::utility::IHostProber> m_pHostProber;
            std::shared_ptr<northstar::utility::ILogger> m_pLogger;
            std::shared_ptr<northstar::openvr::IVRProperties> m_pVRProperties;
            vr::IVRSettings* m_pVRSettings;
            vr::IVRServerDriverHost* m_pVRServerDriverHost;
        };
    }
}