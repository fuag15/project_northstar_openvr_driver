#pragma once
#include <string>
#include <openvr_driver.h>

#include "utility/ILogger.hpp"

namespace northstar {
    namespace utility {
        class CLogger : public ILogger
        {
        public:
            CLogger(vr::IVRDriverLog* driverLog);
            virtual void Log(const char* pchMessage) override final;
            virtual void LogDebug(const char* pchMessage) override final;
        private:
            vr::IVRDriverLog* m_pDriverLog;
        };
    }
}