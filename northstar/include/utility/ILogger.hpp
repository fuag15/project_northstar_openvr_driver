#pragma once
#include <string>
#include <openvr_driver.h>
#include <memory>

namespace northstar {
    namespace utility {
        class ILogger
        {
        public:
            virtual void Log(const char* pchMessage) = 0;
            virtual void LogDebug(const char* pchFormatMessage) = 0;
        };
    }
}
