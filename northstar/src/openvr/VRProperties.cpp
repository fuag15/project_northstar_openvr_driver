#include "openvr/VRProperties.hpp"

northstar::openvr::CVRProperties::CVRProperties(vr::CVRPropertyHelpers* pVRPropertyHelpers) {
    m_sVRPropertyHelpers = pVRPropertyHelpers;
}

inline vr::PropertyContainerHandle_t northstar::openvr::CVRProperties::TrackedDeviceToPropertyContainer(vr::TrackedDeviceIndex_t nDevice) {
    return m_sVRPropertyHelpers->TrackedDeviceToPropertyContainer(nDevice);
}

inline vr::ETrackedPropertyError northstar::openvr::CVRProperties::SetBoolProperty(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, bool bNewValue) {
    return m_sVRPropertyHelpers->SetBoolProperty(ulContainerHandle, prop, bNewValue);
}

inline vr::ETrackedPropertyError northstar::openvr::CVRProperties::SetStringProperty(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, const char* pchNewValue) {
    return m_sVRPropertyHelpers->SetStringProperty(ulContainerHandle, prop, pchNewValue);
}

inline vr::ETrackedPropertyError northstar::openvr::CVRProperties::SetFloatProperty(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, float fNewValue) {
    return m_sVRPropertyHelpers->SetFloatProperty(ulContainerHandle, prop, fNewValue);
}

inline vr::ETrackedPropertyError northstar::openvr::CVRProperties::SetInt32Property(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, int32_t nNewValue) {
    return m_sVRPropertyHelpers->SetInt32Property(ulContainerHandle, prop, nNewValue);
}

inline vr::ETrackedPropertyError northstar::openvr::CVRProperties::SetUint64Property(vr::PropertyContainerHandle_t ulContainerHandle, vr::ETrackedDeviceProperty prop, uint64_t ulNewValue) {
    return m_sVRPropertyHelpers->SetUint64Property(ulContainerHandle, prop, ulNewValue);
}
