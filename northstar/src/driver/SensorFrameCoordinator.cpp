#include "driver/SensorFrameCoordinator.hpp"

using namespace northstar::driver::types;

northstar::driver::CSensorFrameCoordinator::CSensorFrameCoordinator(
    std::shared_ptr<ILeapMotion> pLeapMotion, 
    std::shared_ptr<northstar::utility::ILogger> pLogger) {
    m_pLeapMotion = pLeapMotion;
    m_pLogger = pLogger;
    m_oDriverPose = std::nullopt;
    m_oLeapTrackingEvent = std::nullopt;
}

std::optional<vr::DriverPose_t> northstar::driver::CSensorFrameCoordinator::GetOpenVRHeadPose() {
    return m_oDriverPose;
}

std::optional<LEAP_HAND> northstar::driver::CSensorFrameCoordinator::GetLeapHandPose(const EHand& eHand) {
    if (!m_oDriverPose)
        return std::nullopt;

    if (!m_oLeapTrackingEvent)
        m_oLeapTrackingEvent = m_pLeapMotion->GetFrame();

    if (!m_oLeapTrackingEvent)
        return std::nullopt;

    for (uint32_t i = 0; i < m_oLeapTrackingEvent->nHands; i++) {
        if (m_oLeapTrackingEvent->pHands[i].type
            == (eHand == EHand::Left ? eLeapHandType_Left : eLeapHandType_Right)) {
            return m_oLeapTrackingEvent->pHands[i];
        }
    }

    return std::nullopt;
}

void northstar::driver::CSensorFrameCoordinator::ResetFrameState() {
    m_oDriverPose.reset();
    m_oLeapTrackingEvent.reset();
}

void northstar::driver::CSensorFrameCoordinator::SubmitOpenVRHeadsetPose(const vr::DriverPose_t& sDriverPose) {
    m_oDriverPose.emplace(sDriverPose);
}
