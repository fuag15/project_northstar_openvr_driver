#pragma once

#include <chrono>
#include <atomic>
#include <memory>
#include <thread>
#include <optional>
#include <LeapC.h>

#include "driver/ILeapMotion.hpp"
#include "utility/ILogger.hpp"

namespace northstar {
    namespace driver {
        class CLeapMotion : public ILeapMotion {
        public:
            CLeapMotion(std::shared_ptr<northstar::utility::ILogger> pLogger);
            ~CLeapMotion();
            virtual std::optional<LEAP_TRACKING_EVENT> GetFrame() override final;
            virtual std::optional<LEAP_TRACKING_EVENT> GetInterpolatedFrame() override final;
            virtual bool IsConnected() const override final;

            void PollingUpdateLoop();
        private:
            static constexpr uint64_t x_uiDesiredPolicyFlags =
                eLeapPolicyFlag_BackgroundFrames & eLeapPolicyFlag_OptimizeHMD;

            static constexpr uint64_t x_uiUnwantedPolicyFlags = 
                eLeapPolicyFlag_Images & eLeapPolicyFlag_MapPoints & eLeapPolicyFlag_AllowPauseResume;

            void AttemptToConnect();
            void Disconnect();
            void StartPollingThread();

            static constexpr uint32_t x_unPollingTimeoutInMilliseconds = 1000;
            std::atomic<bool> m_bPollingShouldStop;
            std::atomic<bool> m_bConnected;
            std::atomic<bool> m_bRecievingFrames;
            std::atomic<bool> m_bConfigSet;
            std::atomic<LEAP_TRACKING_EVENT> m_sLastPolledTrackingEvent;
            std::thread m_tPollingThread;
            LEAP_CONNECTION m_sLeapConnection;
            LEAP_CLOCK_REBASER m_sClockSynchronizer;

            std::shared_ptr<northstar::utility::ILogger> m_pLogger;
        };
    }
}