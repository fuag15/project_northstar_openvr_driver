#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>
#include <openvr_driver.h>

#include "openvr/IVRProperties.hpp"

namespace northstar {
    namespace openvr {
        class MockIVRProperties : public IVRProperties {
            MAKE_MOCK1(TrackedDeviceToPropertyContainer, vr::PropertyContainerHandle_t(vr::TrackedDeviceIndex_t), override);
            MAKE_MOCK3(SetBoolProperty, vr::ETrackedPropertyError(vr::PropertyContainerHandle_t, vr::ETrackedDeviceProperty, bool), override);
            MAKE_MOCK3(SetStringProperty, vr::ETrackedPropertyError(vr::PropertyContainerHandle_t, vr::ETrackedDeviceProperty, const char*), override);
            MAKE_MOCK3(SetFloatProperty, vr::ETrackedPropertyError(vr::PropertyContainerHandle_t, vr::ETrackedDeviceProperty, float), override);
            MAKE_MOCK3(SetInt32Property, vr::ETrackedPropertyError(vr::PropertyContainerHandle_t, vr::ETrackedDeviceProperty, int32_t), override);
            MAKE_MOCK3(SetUint64Property, vr::ETrackedPropertyError(vr::PropertyContainerHandle_t, vr::ETrackedDeviceProperty, uint64_t), override);
        };
    }
}
