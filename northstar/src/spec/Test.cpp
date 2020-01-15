#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>
#include <utility/Test.hpp>

DOCTEST_SYMBOL_EXPORT void from_northstar();   // to silence "-Wmissing-declarations" with GCC
DOCTEST_SYMBOL_EXPORT void from_northstar() {} // force the creation of a .lib file with MSVC

northstar::math::types::AffineMatrix4d northstar::test::utils::CreateMatrixFromArray(const std::array<double, 16>& adData) {
    northstar::math::types::AffineMatrix4d m4dTransform;
    m4dTransform.matrix() <<
        adData[0], adData[1], adData[2], adData[3],
        adData[4], adData[5], adData[6], adData[7],
        adData[8], adData[9], adData[10], adData[11],
        adData[12], adData[13], adData[14], adData[15];
    return m4dTransform;
}