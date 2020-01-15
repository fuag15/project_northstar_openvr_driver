#pragma once

#include <openvr_driver.h>
#include <memory>

#include "openvr/IVRProperties.hpp"

namespace northstar {
    namespace openvr {
        class CVRProperties : public IVRProperties {
        public:
            CVRProperties(vr::CVRPropertyHelpers* pVRPropertyHelpers);
            virtual vr::PropertyContainerHandle_t TrackedDeviceToPropertyContainer(vr::TrackedDeviceIndex_t nDevice) override final;
            virtual vr::ETrackedPropertyError SetBoolProperty(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, bool bNewValue) override final;
            virtual vr::ETrackedPropertyError SetStringProperty(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, const char *pchNewValue) override final;
            virtual vr::ETrackedPropertyError SetFloatProperty(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, float fNewValue) override final;
            virtual vr::ETrackedPropertyError SetInt32Property(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, int32_t nNewValue) override final;
            virtual vr::ETrackedPropertyError SetUint64Property(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, uint64_t ulNewValue) override final;

        private:
            vr::CVRPropertyHelpers* m_sVRPropertyHelpers;
        };
    }
}