#include <openvr_driver.h>

#include <thread>

#if defined( _WINDOWS )
#include <windows.h>
#endif

namespace northstar {
    namespace driver {
        class CWatchdog {
        public:
            CWatchdog() = default;
            ~CWatchdog();
            virtual vr::EVRInitError Init(vr::IVRDriverContext *pDriverContext);
            virtual void Cleanup();
            void ThreadFunction();
        private:
            std::thread m_pWatchdogThread;
            bool m_bThreadShouldStop;
        };
    }
}