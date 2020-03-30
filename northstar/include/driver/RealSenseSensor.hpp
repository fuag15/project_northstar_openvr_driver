#pragma once

#include <librealsense2/rs.hpp>
#include <openvr_driver.h>

#include <atomic>
#include <thread>

#include "driver/IEnvironmentSensor.hpp"
#include "utility/Logger.hpp"

namespace northstar {
    namespace driver {
        class CRealSenseSensor : public IEnvironmentSensor
        {
        public:
            CRealSenseSensor(
                std::shared_ptr<northstar::utility::CLogger> pLogger);

            ~CRealSenseSensor();
            virtual bool GetPose(vr::DriverPose_t& pose, EPoseRetrievalError& error) override final;
            virtual bool SessionStartWasSuccessful() override final;

            void PollingUpdateLoop();

        private:
            struct SStatus {
                std::atomic<bool> bDriverSessionIsRunning = false;
            };

            void ConvertRealSensePoseToOpenVRPose(const rs2_pose& rsPose, vr::DriverPose_t& ovrPose);
            void CopyRealSenseSensorVectorIntoDriverPose(const rs2_vector& pfRealSenseVec, double* pdDriverPoseVec) const;
            void StartPollingDriverServerOnBackgroundThread();

            static constexpr size_t s_unPollingSleepIntervalInMilliSeconds = 33;
            std::atomic<rs2_pose> m_LastPoseRecieved;
            std::atomic<bool> m_bPollingShouldStop = false;
            std::thread m_PollingThread;
            SStatus m_Status;
            bool m_bSessionStartSuccessful;

            std::shared_ptr<northstar::utility::CLogger> m_pLogger;
        };
    }
}
