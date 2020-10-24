#include "driver/Watchdog.hpp"

void northstar::driver::CWatchdog::ThreadFunction() {
    while (!m_bThreadShouldStop)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        //vr::VRWatchdogHost()->WatchdogWakeUp();
    }
}

vr::EVRInitError northstar::driver::CWatchdog::Init(vr::IVRDriverContext *pDriverContext) {
    VR_INIT_WATCHDOG_DRIVER_CONTEXT(pDriverContext);
    m_bThreadShouldStop = false;
    m_pWatchdogThread = std::thread([this] { ThreadFunction(); });
    return vr::VRInitError_None;
}

void northstar::driver::CWatchdog::Cleanup() {
    m_bThreadShouldStop = true;
    if (m_pWatchdogThread.joinable())
        m_pWatchdogThread.join();
}

northstar::driver::CWatchdog::~CWatchdog() {
    Cleanup();
}