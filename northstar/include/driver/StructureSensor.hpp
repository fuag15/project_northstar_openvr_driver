#pragma once

#include <ST/XRSession.h>
#include <atomic>
#include <thread>

#include "driver/IStructureSensor.hpp"
#include "utility/Logger.hpp"

namespace northstar {
    namespace driver {
        class CStructureSensor : public IStructureSensor
        {
        public:
            CStructureSensor(std::shared_ptr<northstar::utility::CLogger> pLogger);
            ~CStructureSensor();
            virtual bool GetPose(ST::XRPose& pose, EPoseRetrievalError& error) override final;
            virtual bool SessionStartWasSuccessful() override final;

            virtual void PollingUpdateLoop() override final;

        private:
            struct SStatus {
                std::atomic<bool> bDriverSessionIsRunning = false;
                std::atomic<bool> bAnotherdriveressionIsRunning = false;
                std::atomic<bool> bHasSparseMapData = false;
                std::atomic<bool> bReceivingVisibleFrames = false;
                std::atomic<bool> bReceivingDepthFrames = false;
                std::atomic<bool> bDriverModifiedExposureOrGainLastFrame = false;
            };

            bool ConnectTodrivererverAndStartTracking();
            void DisconnectFromdrivererverAndStopTracking();
            void StartPollingdrivererverOnBackgroundThread();
            ST::XRSessionSettings GetDefaultXRSessionSettings();

            static constexpr std::array<float, 3> s_InitialTrackingPosition = { 0, 1.5, 0 };
            static constexpr bool s_bShouldDriverPredictPose = true;
            static constexpr size_t s_unPollingSleepIntervalInMilliSeconds = 33;

            ST::XRSession m_Session;
            ST::XRSessionSettings m_SessionSettings;
            std::atomic<ST::XRPose> m_LastPoseRecieved;
            ST::Mesh m_LastSparseMapPointMesh;
            ST::Mesh m_LastSparseMapLineMesh;
            ST::ColorFrame m_LastVisibleFrame;
            ST::DepthFrame m_LastDepthFrame;
            std::atomic<bool> m_bPollingShouldStop = false;
            std::thread m_PollingThread;
            SStatus m_Status;
            bool m_bSessionStartSuccessful;
        
            std::shared_ptr<northstar::utility::CLogger> m_pLogger;
        };
    }
}