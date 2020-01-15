#pragma once

#include "utility/IHostProber.hpp"

#include <optional>

#if defined(_WIN32)
#include <Windows.h>
#endif

namespace northstar {
    namespace utility {
        class CHostProber : public IHostProber {
        public:
            CHostProber() = default;
            virtual std::optional<long> ProbeDisplayOriginX() const override final;
        private:
            static constexpr long x_lTargetDisplayWidth = 2880;
            static constexpr long x_lTargetDisplayHeight = 1600;

#if defined(_WIN32)
            static BOOL CALLBACK InfoEnumProc(HMONITOR hMonitor, HDC hdc, LPRECT lprcMonitor, LPARAM pData);
            static std::optional<long> m_slDisplayOriginX;
#endif
        };
    }
}