#include "driver/RealSenseSensor.hpp"

northstar::driver::CRealSenseSensor::CRealSenseSensor(
    std::shared_ptr<northstar::utility::CLogger> pLogger)
{
    m_pLogger = pLogger;
    m_bSessionStartSuccessful = true;
	StartPollingDriverServerOnBackgroundThread();
}

void northstar::driver::CRealSenseSensor::StartPollingDriverServerOnBackgroundThread()
{
    m_PollingThread = std::thread([this] { PollingUpdateLoop(); });
}

northstar::driver::CRealSenseSensor::~CRealSenseSensor()
{
    m_bPollingShouldStop = true;
    if (m_PollingThread.joinable())
        m_PollingThread.join();
}

bool northstar::driver::CRealSenseSensor::GetPose(
    vr::DriverPose_t& pose, 
    EPoseRetrievalError& error) {
    const rs2_pose rsPose = m_LastPoseRecieved;
    ConvertRealSensePoseToOpenVRPose(rsPose, pose);
    if (!SessionStartWasSuccessful()) {
        error = EPoseRetrievalError::SessionStartFailed;
        return false;
    } else if (!m_Status.bDriverSessionIsRunning) {
        error = EPoseRetrievalError::SessionNotRunning;
        return false;
    }

    return true;
}

// TODO: Time offset
void northstar::driver::CRealSenseSensor::ConvertRealSensePoseToOpenVRPose(const rs2_pose& rsPose, vr::DriverPose_t& ovrPose) {
    ovrPose.willDriftInYaw = false;
    ovrPose.shouldApplyHeadModel = false;

    ovrPose.qWorldFromDriverRotation = vr::HmdQuaternion_t{ 1, 0, 0, 0 };
    ovrPose.vecWorldFromDriverTranslation[0] = 0;
    ovrPose.vecWorldFromDriverTranslation[1] = 0;
    ovrPose.vecWorldFromDriverTranslation[2] = 0;

    // TODO: measure distance / orientation from head to structure sensors rotation
    ovrPose.qDriverFromHeadRotation = vr::HmdQuaternion_t{ 1, 0, 0, 0 };
    ovrPose.vecDriverFromHeadTranslation[0] = 0;
    ovrPose.vecDriverFromHeadTranslation[1] = 0;
    ovrPose.vecDriverFromHeadTranslation[2] = 0;

    ovrPose.qRotation.w = rsPose.rotation.w;
    ovrPose.qRotation.x = rsPose.rotation.x;
    ovrPose.qRotation.y = rsPose.rotation.y;
    ovrPose.qRotation.z = rsPose.rotation.z;

    CopyRealSenseSensorVectorIntoDriverPose(rsPose.translation, ovrPose.vecPosition);

    // TODO: These are choppy
    CopyRealSenseSensorVectorIntoDriverPose(/*rsPose.velocity*/{ 0, 0, 0 }, ovrPose.vecVelocity);
    CopyRealSenseSensorVectorIntoDriverPose(/*rsPose.acceleration*/{ 0, 0, 0 }, ovrPose.vecAcceleration);
    CopyRealSenseSensorVectorIntoDriverPose(/*rsPose.angular_velocity*/{ 0, 0, 0 }, ovrPose.vecAngularVelocity);
    CopyRealSenseSensorVectorIntoDriverPose(/*rsPose.angular_acceleration*/{ 0, 0, 0 }, ovrPose.vecAngularAcceleration);
}

void northstar::driver::CRealSenseSensor::CopyRealSenseSensorVectorIntoDriverPose(const rs2_vector& rsVector, double* pdDriverPoseVec) const {
    pdDriverPoseVec[0] = rsVector.x;
    pdDriverPoseVec[1] = rsVector.y;
    pdDriverPoseVec[2] = rsVector.z;
}

bool northstar::driver::CRealSenseSensor::SessionStartWasSuccessful() {
    return m_bSessionStartSuccessful;
}

// TODO: Driver start success etc
void northstar::driver::CRealSenseSensor::PollingUpdateLoop() 
{
    try {
        rs2::pipeline rsPipeLine;
        rs2::config rsConfig;
        rsConfig.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);
        rsPipeLine.start(rsConfig);

        while (!m_bPollingShouldStop)
        {
            auto sFrameSet = rsPipeLine.wait_for_frames();
            auto sFrame = sFrameSet.first_or_default(RS2_STREAM_POSE);
            m_LastPoseRecieved = sFrame.as<rs2::pose_frame>().get_pose_data();
            m_Status.bDriverSessionIsRunning = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(s_unPollingSleepIntervalInMilliSeconds));
        }
    } catch (const rs2::error&) {
        m_Status.bDriverSessionIsRunning = false;
    } catch (const std::exception&) {
        m_Status.bDriverSessionIsRunning = false;
    }
}
