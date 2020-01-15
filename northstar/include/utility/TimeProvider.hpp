#pragma once

#include "utility/ITimeProvider.hpp"

namespace northstar {
    namespace utility {
        class CTimeProvider : public ITimeProvider {
        public:
            virtual double CurrentUnixTimestamp() const;
        };
    }
}