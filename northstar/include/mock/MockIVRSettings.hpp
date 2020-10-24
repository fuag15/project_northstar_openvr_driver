#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include <openvr_driver.h>

namespace northstar {
    namespace openvr {
        class MockIVRSettings : public vr::IVRSettings {
        public:
            MAKE_MOCK1(GetSettingsErrorNameFromEnum, const char*( vr::EVRSettingsError eError ), override);
            MAKE_MOCK4(SetBool, void(const char* pchSection, const char* pchSettingsKey, bool bValue, vr::EVRSettingsError* peError), override);
            MAKE_MOCK4(SetInt32, void( const char *pchSection, const char *pchSettingsKey, int32_t nValue, vr::EVRSettingsError *peError ), override);
            MAKE_MOCK4(SetFloat, void( const char *pchSection, const char *pchSettingsKey, float flValue, vr::EVRSettingsError *peError ), override);
            MAKE_MOCK4(SetString, void( const char *pchSection, const char *pchSettingsKey, const char *pchValue, vr::EVRSettingsError *peError ), override);
            MAKE_MOCK3(GetBool, bool( const char *pchSection, const char *pchSettingsKey, vr::EVRSettingsError *peError ), override);
            MAKE_MOCK3(GetInt32, int32_t( const char *pchSection, const char *pchSettingsKey, vr::EVRSettingsError *peError ), override);
            MAKE_MOCK3(GetFloat, float( const char *pchSection, const char *pchSettingsKey, vr::EVRSettingsError *peError ), override);
            MAKE_MOCK5(GetString, void( const char *pchSection, const char *pchSettingsKey, VR_OUT_STRING() char *pchValue, uint32_t unValueLen, vr::EVRSettingsError *peError ), override);
            MAKE_MOCK2(RemoveSection, void( const char *pchSection, vr::EVRSettingsError *peError ), override);
            MAKE_MOCK3(RemoveKeyInSection, void( const char *pchSection, const char *pchSettingsKey, vr::EVRSettingsError *peError ), override);
        };
    }
}
