#include "driver/Factory.hpp"

HMD_EXPORT void* HmdDriverFactory(const char *pInterfaceName, int *pReturnCode) {
    if (std::string(pInterfaceName) == std::string(vr::IServerTrackedDeviceProvider_Version))
        return new northstar::driver::CServer();

    if (pReturnCode)
        *pReturnCode = vr::VRInitError_Init_InterfaceNotFound;

    return nullptr;
}