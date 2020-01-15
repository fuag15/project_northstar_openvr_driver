#include "driver/Watchdog.hpp"

void northstar::driver::CWatchdog::ThreadFunction() {
    while (!m_bThreadShouldStop)
    {
#if defined( _WINDOWS )
        if ((0x01 & GetAsyncKeyState('Y')) != 0) {
            vr::VRWatchdogHost()->WatchdogWakeUp();
        }

        std::this_thread::sleep_for(std::chrono::microseconds(500));
#else
        std::this_thread::sleep_for(std::chrono::seconds(5));
        vr::VRWatchdogHost()->WatchdogWakeUp();
#endif
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