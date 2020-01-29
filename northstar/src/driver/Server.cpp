#include "driver/Server.hpp"

vr::EVRInitError northstar::driver::CServer::Init(vr::IVRDriverContext* pDriverContext) {
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
    m_pLogger = std::make_shared<northstar::utility::CLogger>(vr::VRDriverLog());
    m_pVRProperties = std::make_shared<northstar::openvr::CVRProperties>(vr::VRProperties());
    m_pHostProber = std::make_shared<northstar::utility::CHostProber>();
    m_pMatrixFactory = std::make_shared<northstar::math::CMatrixFactory>();
    m_pVectorFactory = std::make_shared<northstar::math::CVectorFactory>();
    m_pTimeProvider = std::make_shared<northstar::utility::CTimeProvider>();
    m_pSpaceAdapter = std::make_shared<northstar::math::CSpaceAdapter>(m_pVectorFactory);
    m_pGeometry = std::make_shared <northstar::math::CGeometry>(m_pVectorFactory);
    m_pSkeletalAdapter = std::make_shared<northstar::math::CSkeletalAdapter>(m_pVectorFactory, m_pMatrixFactory);
    m_pLeapMotion = std::make_shared<northstar::driver::CLeapMotion>(m_pLogger);
    m_pSensorFrameCoordinator = std::make_shared<northstar::driver::CSensorFrameCoordinator>(m_pLeapMotion, m_pLogger);
    m_pWorldAdapter = std::make_shared<northstar::math::CWorldAdapter>(
        m_pMatrixFactory,
        m_pVectorFactory);

    m_pOptics = std::make_shared<northstar::driver::COptics>(
        vr::VRSettings(),
        m_pWorldAdapter,
        m_pSpaceAdapter,
        m_pGeometry,
        m_pMatrixFactory,
        m_pVectorFactory,
        m_pLogger);

    m_pStructureSensor = std::make_shared<northstar::driver::CStructureSensor>(m_pLogger);
    if (m_pStructureSensor->SessionStartWasSuccessful() || x_bControllerDebugMode) {
        m_pHMD = std::make_unique<northstar::driver::CHMD>(
            vr::VRSettings(),
            vr::VRServerDriverHost(),
            m_pHostProber,
            m_pVRProperties,
            m_pStructureSensor,
            m_pWorldAdapter,
            m_pVectorFactory,
            m_pOptics,
            m_pSensorFrameCoordinator,
            m_pTimeProvider,
            m_pLogger);

        vr::VRServerDriverHost()->TrackedDeviceAdded(
            m_pHMD->GetSerialNumber().data(),
            vr::TrackedDeviceClass_HMD,
            m_pHMD.get());

        for (const auto& eHand : x_aeHands) {
            m_pControllers.push_back(
                std::make_unique<CController>(
                    m_pSensorFrameCoordinator,
                    m_pWorldAdapter,
                    m_pVectorFactory,
                    m_pSkeletalAdapter,
                    m_pLogger,
                    m_pVRProperties,
                    vr::VRSettings(),
                    vr::VRServerDriverHost(),
                    vr::VRDriverInput(),
                    eHand));

            vr::VRServerDriverHost()->TrackedDeviceAdded(
                m_pControllers.back()->GetSerialNumber().data(),
                vr::TrackedDeviceClass_Controller,
                m_pControllers.back().get());
        }
    }

    return vr::EVRInitError::VRInitError_None;
}

void northstar::driver::CServer::Cleanup() {
    m_pLogger = nullptr;
    m_pVRProperties = nullptr;
    m_pHostProber = nullptr;
    m_pMatrixFactory = nullptr;
    m_pVectorFactory = nullptr;
    m_pTimeProvider = nullptr;
    m_pSpaceAdapter = nullptr;
    m_pGeometry = nullptr;
    m_pSkeletalAdapter = nullptr;
    m_pLeapMotion = nullptr;
    m_pSensorFrameCoordinator = nullptr;
    m_pWorldAdapter = nullptr;
    m_pOptics = nullptr;
    m_pStructureSensor = nullptr;
    m_pHMD = nullptr;
    m_pControllers.clear();
}

const char* const* northstar::driver::CServer::GetInterfaceVersions() { 
    return vr::k_InterfaceVersions;
}

void northstar::driver::CServer::RunFrame() {
    if (m_pSensorFrameCoordinator)
        m_pSensorFrameCoordinator->ResetFrameState();

    if (m_pHMD)
        m_pHMD->RunFrame();

    for (auto& controller : m_pControllers)
        controller->RunFrame();

    static vr::VREvent_t vrEvent;
    while (vr::VRServerDriverHost()->PollNextEvent(&vrEvent, sizeof(vrEvent))) {}
}

bool northstar::driver::CServer::ShouldBlockStandbyMode() { 
    return x_bShouldBlockStandbyMode; 
}

void northstar::driver::CServer::EnterStandby() {}

void northstar::driver::CServer::LeaveStandby() {}