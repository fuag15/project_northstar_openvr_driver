#include "driver/LeapMotion.hpp"

using namespace std::chrono;

// TODO: check for result of policy set
northstar::driver::CLeapMotion::CLeapMotion(std::shared_ptr<northstar::utility::ILogger> pLogger) {
    m_bConnected = false;
    m_bConfigSet = false;
    m_bPollingShouldStop = false;
    m_bRecievingFrames = false;
    m_sLastPolledTrackingEvent = { 0 };
    m_sLeapConnection = { 0 };
    m_sClockSynchronizer = { 0 };

    m_pLogger = pLogger;

    AttemptToConnect();
    StartPollingThread();
    LeapCreateClockRebaser(&m_sClockSynchronizer);
}

void northstar::driver::CLeapMotion::AttemptToConnect() {
    if (LeapCreateConnection(nullptr, &m_sLeapConnection) == eLeapRS_Success
        && LeapOpenConnection(m_sLeapConnection) == eLeapRS_Success)
        m_bConnected = true;
}

void northstar::driver::CLeapMotion::StartPollingThread() {
    m_tPollingThread = std::thread([this] { PollingUpdateLoop(); });
}

bool northstar::driver::CLeapMotion::IsConnected() const {
    return m_bConnected;
}

std::optional<LEAP_TRACKING_EVENT> northstar::driver::CLeapMotion::GetFrame() {
    if (!m_bConnected || !m_bRecievingFrames || !m_bConfigSet)
        return std::nullopt;

    return m_sLastPolledTrackingEvent;
}

std::optional<LEAP_TRACKING_EVENT> northstar::driver::CLeapMotion::GetInterpolatedFrame() {
    static int64_t nTargetFrameTime = 0;
    static uint64_t unTargetFrameSize = 0;

    if (!m_bConnected || !m_bRecievingFrames)
        return std::nullopt;

    std::optional<LEAP_TRACKING_EVENT> osTrackingEvent = std::nullopt;
    microseconds nMicrosecondsSinceEpoch = time_point_cast<microseconds>(
        high_resolution_clock::now()).time_since_epoch();

    LeapUpdateRebase(
        m_sClockSynchronizer,
        nMicrosecondsSinceEpoch.count(),
        LeapGetNow());

    LeapRebaseClock(
        m_sClockSynchronizer,
        nMicrosecondsSinceEpoch.count(),
        &nTargetFrameTime);

    if (LeapGetFrameSize(m_sLeapConnection, nTargetFrameTime, &unTargetFrameSize) == eLeapRS_Success) {
        LEAP_TRACKING_EVENT* sInterpolatedFrame = static_cast<LEAP_TRACKING_EVENT*>(
            malloc(static_cast<std::size_t>(unTargetFrameSize)));

        if (LeapInterpolateFrame(
            m_sLeapConnection, 
            nTargetFrameTime, 
            sInterpolatedFrame, 
            unTargetFrameSize) == eLeapRS_Success) {
            osTrackingEvent = *sInterpolatedFrame;
        }

        free(sInterpolatedFrame);
    }

    return osTrackingEvent;
}

void northstar::driver::CLeapMotion::PollingUpdateLoop() 
{
    static LEAP_CONNECTION_MESSAGE s_sLeapConnectionMessage;
    while (!m_bPollingShouldStop)
    {
        if (m_bConnected)
        {
            if (LeapPollConnection(
                m_sLeapConnection, 
                x_unPollingTimeoutInMilliseconds, 
                &s_sLeapConnectionMessage) == eLeapRS_Success) {
                if (s_sLeapConnectionMessage.type == eLeapEventType_Tracking) {
                    m_sLastPolledTrackingEvent = *s_sLeapConnectionMessage.tracking_event;
                    m_bRecievingFrames = true;
                }
            } else {
                m_bRecievingFrames = false;
            }

            if (!m_bConfigSet && m_bRecievingFrames) {
                LeapSetPolicyFlags(m_sLeapConnection, x_uiDesiredPolicyFlags, x_uiUnwantedPolicyFlags);
                m_bConfigSet = true;
            }
        }
    }
}

northstar::driver::CLeapMotion::~CLeapMotion()
{
    Disconnect();
    LeapDestroyConnection(m_sLeapConnection);
    LeapDestroyClockRebaser(m_sClockSynchronizer);
    m_bPollingShouldStop = true;

    if (m_tPollingThread.joinable())
        m_tPollingThread.join();
}

void northstar::driver::CLeapMotion::Disconnect()
{
    if (m_bConnected) {
        m_bConnected = false;
        LeapCloseConnection(m_sLeapConnection);
    }

    m_bConfigSet = false;
}
