#include "utility/HostProber.hpp"

#if defined(_WIN32)
// TODO: This doesn't seem to be causing issues but it is likely querying the wrong display 
std::optional<long> northstar::utility::CHostProber::m_slDisplayOriginX = std::nullopt;
BOOL CALLBACK northstar::utility::CHostProber::InfoEnumProc(HMONITOR hMonitor, HDC hdc, LPRECT lprcMonitor, LPARAM pData) {
    MONITORINFOEX info = { 0 };
    info.cbSize = sizeof(info);
    if (GetMonitorInfo(hMonitor, &info)
        && (info.rcMonitor.right - info.rcMonitor.left) == x_lTargetDisplayWidth
        && (info.rcMonitor.bottom - info.rcMonitor.top) == x_lTargetDisplayHeight) {
        m_slDisplayOriginX = info.rcMonitor.left;
    }

    return TRUE;
}
#endif 

// TODO: clean this up this is likely not querying the right display
std::optional<long> northstar::utility::CHostProber::ProbeDisplayOriginX() const {
#if defined(_WIN32)
    m_slDisplayOriginX.reset();
    EnumDisplayMonitors(
        nullptr,
        nullptr,
        InfoEnumProc,
        0);
    return m_slDisplayOriginX;
#else
    // TODO: XCB equivalent
    return std::nullopt;
#endif
}
