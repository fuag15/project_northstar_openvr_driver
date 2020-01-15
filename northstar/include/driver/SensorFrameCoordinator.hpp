#pragma once

#include <memory>
#include <optional>

#include <LeapC.h>
#include <openvr_driver.h>

#include "driver/ISensorFrameCoordinator.hpp"
#include "driver/ILeapMotion.hpp"
#include "utility/ILogger.hpp"

namespace northstar {
    namespace driver {
        class CSensorFrameCoordinator : public ISensorFrameCoordinator{
        public:
            CSensorFrameCoordinator(std::shared_ptr<ILeapMotion> pLeapMotion, std::shared_ptr<northstar::utility::ILogger> pLogger);
            virtual std::optional<vr::DriverPose_t> GetOpenVRHeadPose() override final;
            virtual std::optional<LEAP_HAND> GetLeapHandPose(const northstar::driver::types::EHand& eHand) override final;
            virtual void ResetFrameState() override final;
            virtual void SubmitOpenVRHeadsetPose(const vr::DriverPose_t& sDriverPose) override final;
        private:
            std::optional<vr::DriverPose_t> m_oDriverPose;
            std::optional<LEAP_TRACKING_EVENT> m_oLeapTrackingEvent;
            std::shared_ptr<ILeapMotion> m_pLeapMotion;
            std::shared_ptr<northstar::utility::ILogger> m_pLogger;
        };
    }
}