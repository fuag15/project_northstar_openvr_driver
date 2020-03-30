#pragma once

#include <openvr_driver.h>

#include "driver/HMD.hpp"
#include "driver/Controller.hpp"
#include "driver/Optics.hpp"
#include "driver/SensorFrameCoordinator.hpp"
#include "driver/LeapMotion.hpp"
#include "driver/IEnvironmentSensor.hpp"
#include "driver/StructureSensor.hpp"
#include "driver/RealSenseSensor.hpp"
#include "driver/Types.hpp"
#include "openvr/VRProperties.hpp"
#include "math/WorldAdapter.hpp"
#include "math/MatrixFactory.hpp"
#include "math/VectorFactory.hpp"
#include "math/SkeletalAdapter.hpp"
#include "math/Geometry.hpp"
#include "math/SpaceAdapter.hpp"
#include "utility/TimeProvider.hpp"
#include "utility/HostProber.hpp"

namespace northstar {
    namespace driver {
        class CServer: public vr::IServerTrackedDeviceProvider
        {
        public:
            CServer() = default;
            virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext);
            virtual void Cleanup() override final;
            virtual const char* const* GetInterfaceVersions() override final;
            virtual void RunFrame() override final;
            virtual bool ShouldBlockStandbyMode() override final;
            virtual void EnterStandby() override final;
            virtual void LeaveStandby() override final;

        private:
            enum class EEnvironmentSensor {
                StructureCore,
                RealSenseT265,
            };

            static constexpr bool x_bControllerDebugMode = true; // TODO: read this from config
            static constexpr bool x_bShouldBlockStandbyMode = true; // TODO: read this from config
            static constexpr EEnvironmentSensor x_eSelectedEnvironmentSensor = EEnvironmentSensor::RealSenseT265; // TODO: read this from config
            static constexpr std::array<northstar::driver::types::EHand, 2> x_aeHands = { northstar::driver::types::EHand::Left, northstar::driver::types::EHand::Right };

            std::shared_ptr<northstar::math::CVectorFactory> m_pVectorFactory;
            std::shared_ptr<northstar::math::CMatrixFactory> m_pMatrixFactory;
            std::shared_ptr<northstar::math::CWorldAdapter> m_pWorldAdapter;
            std::shared_ptr<northstar::math::CSpaceAdapter> m_pSpaceAdapter;
            std::shared_ptr<northstar::math::CSkeletalAdapter> m_pSkeletalAdapter;
            std::shared_ptr<northstar::math::CGeometry> m_pGeometry;
            std::shared_ptr<northstar::utility::CLogger> m_pLogger;
            std::shared_ptr<northstar::utility::CTimeProvider> m_pTimeProvider;
            std::shared_ptr<northstar::utility::CHostProber> m_pHostProber;
            std::shared_ptr<northstar::driver::CLeapMotion> m_pLeapMotion;
            std::shared_ptr<northstar::driver::IEnvironmentSensor> m_pEnvironmentSensor;
            std::shared_ptr<northstar::driver::CSensorFrameCoordinator> m_pSensorFrameCoordinator;
            std::shared_ptr<northstar::driver::COptics> m_pOptics;
            std::unique_ptr<northstar::driver::CHMD> m_pHMD;
            std::vector<std::unique_ptr<northstar::driver::CController>> m_pControllers;
            std::shared_ptr<northstar::openvr::CVRProperties> m_pVRProperties;
        };
    }
}