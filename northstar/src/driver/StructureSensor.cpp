#include "driver/StructureSensor.hpp"

northstar::driver::CStructureSensor::CStructureSensor(std::shared_ptr<northstar::utility::CLogger> pLogger)
{
    m_pLogger = pLogger;
    m_SessionSettings = GetDefaultXRSessionSettings();
    m_bSessionStartSuccessful = ConnectTodrivererverAndStartTracking();
    if (m_bSessionStartSuccessful)
        StartPollingdrivererverOnBackgroundThread();
}

ST::XRSessionSettings northstar::driver::CStructureSensor::GetDefaultXRSessionSettings()
{
    ST::XRSessionSettings settings;
    settings.trackingMode = ST::XRTrackingMode::Default;
    settings.initialPose = ST::Matrix4::identity().translated(
        s_InitialTrackingPosition[0],
        s_InitialTrackingPosition[1],
        s_InitialTrackingPosition[2]);

    return settings;
}

bool northstar::driver::CStructureSensor::ConnectTodrivererverAndStartTracking()
{
    if (m_Session.connectToServer() != ST::XRStatus::Good)
        return false;

    if (m_Session.startTracking(m_SessionSettings) != ST::XRStatus::Good)
        return false;

    return true;
}

void northstar::driver::CStructureSensor::StartPollingdrivererverOnBackgroundThread()
{
    m_PollingThread = std::thread([this] { PollingUpdateLoop(); });
}

northstar::driver::CStructureSensor::~CStructureSensor()
{
    m_bPollingShouldStop = true;
    if (m_PollingThread.joinable())
        m_PollingThread.join();

    DisconnectFromdrivererverAndStopTracking();
}

void northstar::driver::CStructureSensor::DisconnectFromdrivererverAndStopTracking()
{
    m_Session.stopTracking();
    m_Session.disconnectFromServer();
    m_LastPoseRecieved = ST::XRPose();
}

bool northstar::driver::CStructureSensor::GetPose(
    ST::XRPose& pose, 
    EPoseRetrievalError& error) {
    pose = m_LastPoseRecieved;
    if (!SessionStartWasSuccessful()) {
        error = EPoseRetrievalError::SessionStartFailed;
        return false;
    }
    else if (!m_Status.bDriverSessionIsRunning) {
        error = EPoseRetrievalError::SessionNotRunning;
        return false;
    }
    else if (!pose.status.motionIsInitialized) {
        error = EPoseRetrievalError::SessionInitializing;
        return false;
    }
    else if (!pose.isValid()
             || !pose.status.cameraSuccessfullyTracked) {
        error = EPoseRetrievalError::InvalidPoseRecieved;
        return false;
    }

    return true;
}

bool northstar::driver::CStructureSensor::SessionStartWasSuccessful()
{
    return m_bSessionStartSuccessful;
}

void northstar::driver::CStructureSensor::PollingUpdateLoop() 
{
    while (!m_bPollingShouldStop)
    {
        if (m_Session.isTrackingRunning())
        {
            ST::XRPose pose;
            ST::XRFrameOfReference referenceFrame = ST::XRFrameOfReference::VisibleCamera;
            if (s_bShouldDriverPredictPose)
                m_Session.predictWorldFromCameraPose(referenceFrame, m_Session.currentTime(), pose);
            else
                m_Session.readLastWorldFromCameraPose(referenceFrame, pose, 0.0);

            m_LastPoseRecieved = pose;
            m_Status.bHasSparseMapData = (m_Session.getSparseMap(m_LastSparseMapPointMesh, m_LastSparseMapLineMesh) == ST::XRStatus::Good);
            {
                {
                    const bool couldReadFrame = m_Session.readLastVisibleFrame(m_LastVisibleFrame, m_LastVisibleFrame.timestamp()) == ST::XRStatus::Good;
                    const bool frameIsInvalid = m_LastVisibleFrame.isValid() == false;
                    m_Status.bReceivingVisibleFrames = couldReadFrame && !frameIsInvalid;
                }
                {
                    const bool couldReadFrame = m_Session.readLastDepthFrame(m_LastDepthFrame, m_LastDepthFrame.timestamp()) == ST::XRStatus::Good;
                    const bool frameIsInvalid = m_LastDepthFrame.isValid() == false;
                    m_Status.bReceivingDepthFrames = couldReadFrame && !frameIsInvalid;
                }
            }
        }

        m_Status.bDriverSessionIsRunning = m_Session.isTrackingRunning();
        m_Status.bAnotherdriveressionIsRunning = ST::XRSession::isThereASessionRunningInAnotherProcess();
        std::this_thread::sleep_for(std::chrono::milliseconds(s_unPollingSleepIntervalInMilliSeconds));
    }
}
