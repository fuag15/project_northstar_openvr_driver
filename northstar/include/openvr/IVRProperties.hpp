#pragma once

#include <openvr_driver.h>

namespace northstar {
    namespace openvr {
        class IVRProperties {
        public:
            virtual vr::PropertyContainerHandle_t TrackedDeviceToPropertyContainer(vr::TrackedDeviceIndex_t nDevice) = 0;
            virtual vr::ETrackedPropertyError SetBoolProperty(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, bool bNewValue) = 0;
            virtual vr::ETrackedPropertyError SetStringProperty(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, const char *pchNewValue) = 0;
            virtual vr::ETrackedPropertyError SetFloatProperty(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, float fNewValue) = 0;
            virtual vr::ETrackedPropertyError SetInt32Property(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, int32_t nNewValue) = 0;
            virtual vr::ETrackedPropertyError SetUint64Property(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, uint64_t ulNewValue) = 0;
        };
    }
}