#include "driver/StructureSensor.hpp"

northstar::driver::CStructureSensor::CStructureSensor(
    std::shared_ptr<northstar::math::IWorldAdapter> pWorldAdapter,
    std::shared_ptr<northstar::utility::ITimeProvider> pTimeProvider,
    std::shared_ptr<northstar::utility::CLogger> pLogger)
{
    m_pWorldAdapter = pWorldAdapter;
    m_pTimeProvider = pTimeProvider;
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
    vr::DriverPose_t& pose, 
    EPoseRetrievalError& error) {
    const ST::XRPose xrPose = m_LastPoseRecieved;
    ConvertXRPoseToOpenVRPose(xrPose, pose);
    if (!SessionStartWasSuccessful()) {
        error = EPoseRetrievalError::SessionStartFailed;
        return false;
    }
    else if (!m_Status.bDriverSessionIsRunning) {
        error = EPoseRetrievalError::SessionNotRunning;
        return false;
    }
    else if (!xrPose.status.motionIsInitialized) {
        error = EPoseRetrievalError::SessionInitializing;
        return false;
    }
    else if (!xrPose.isValid()
             || !xrPose.status.cameraSuccessfullyTracked) {
        error = EPoseRetrievalError::InvalidPoseRecieved;
        return false;
    }

    return true;
}

void northstar::driver::CStructureSensor::ConvertXRPoseToOpenVRPose(const ST::XRPose& xrPose, vr::DriverPose_t& ovrPose) {
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

    auto positionAndOrientation = m_pWorldAdapter->
        FromStructureSensorPoseToOpenVRSpace(xrPose);

    ovrPose.vecPosition[0] = positionAndOrientation.position.x();
    ovrPose.vecPosition[1] = positionAndOrientation.position.y();
    ovrPose.vecPosition[2] = positionAndOrientation.position.z();

    ovrPose.qRotation.w = positionAndOrientation.orientation.w();
    ovrPose.qRotation.x = positionAndOrientation.orientation.x();
    ovrPose.qRotation.y = positionAndOrientation.orientation.y();
    ovrPose.qRotation.z = positionAndOrientation.orientation.z();

    CopyStructureSensorLinearVectorIntoDriverPose(ovrPose.vecVelocity, xrPose.linearVelocity);
    CopyStructureSensorLinearVectorIntoDriverPose(ovrPose.vecAcceleration, xrPose.linearAcceleration);
    CopyStructureSensorAngularVectorIntoDriverPose(ovrPose.vecAngularVelocity, xrPose.angularVelocity);
    CopyStructureSensorAngularVectorIntoDriverPose(ovrPose.vecAngularAcceleration, xrPose.angularAcceleration);

    ovrPose.poseTimeOffset = m_pTimeProvider->CurrentUnixTimestamp() - xrPose.timestamp;
}

// TODO: investigate / fix this is very choppy
void northstar::driver::CStructureSensor::CopyStructureSensorLinearVectorIntoDriverPose(double* pdDriverPoseVec, const float* pfXRPoseVec) const {
    //auto v3dOpenVRVec = 
    //    m_pWorldAdapter->FromStructureSensorLinearVectorArrayToOpenVRSpace(
    //        std::array<double, 3> {
    //            pfXRPoseVec[0],
    //            pfXRPoseVec[1],
    //            pfXRPoseVec[2]});

    //pdDriverPoseVec[0] = v3dOpenVRVec.x();
    //pdDriverPoseVec[1] = v3dOpenVRVec.y();
    //pdDriverPoseVec[2] = v3dOpenVRVec.z();
    pdDriverPoseVec[0] = 0;
    pdDriverPoseVec[1] = 0;
    pdDriverPoseVec[2] = 0;
}

// TODO: investigate / fix this is very choppy
void northstar::driver::CStructureSensor::CopyStructureSensorAngularVectorIntoDriverPose(double* pdDriverPoseVec, const float* pfXRPoseVec) const {
    //auto v3dOpenVRVec = 
    //    m_pWorldAdapter->FromStructureSensorAngularVectorArrayToOpenVRSpace(
    //        std::array<double, 3> {
    //            pfXRPoseVec[0],
    //            pfXRPoseVec[1],
    //            pfXRPoseVec[2]});

    //pdDriverPoseVec[0] = v3dOpenVRVec.x();
    //pdDriverPoseVec[1] = v3dOpenVRVec.y();
    //pdDriverPoseVec[2] = v3dOpenVRVec.z();
    pdDriverPoseVec[0] = 0;
    pdDriverPoseVec[1] = 0;
    pdDriverPoseVec[2] = 0;
}

bool northstar::driver::CStructureSensor::SessionStartWasSuccessful() {
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
