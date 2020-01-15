#pragma once

#include <openvr_driver.h>

#include <string>

#include "driver/Server.hpp"
#include "utility/Preprocessor.hpp"

HMD_DLL_EXPORT void* HmdDriverFactory(const char *pInterfaceName, int *pReturnCode);