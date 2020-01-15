#include "utility/TimeProvider.hpp"
#include <ctime>

double northstar::utility::CTimeProvider::CurrentUnixTimestamp() const {
    return static_cast<double>(std::time(nullptr));
}
