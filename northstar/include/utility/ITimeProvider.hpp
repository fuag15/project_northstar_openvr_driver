#pragma once

namespace northstar {
    namespace utility {
        class ITimeProvider {
        public:
            virtual double CurrentUnixTimestamp() const = 0;
        };
    }
}