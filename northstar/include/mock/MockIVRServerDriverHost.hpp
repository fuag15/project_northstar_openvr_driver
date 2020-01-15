#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include <openvr_driver.h>

namespace northstar {
    namespace openvr {
        class MockIVRServerDriverHost: public vr::IVRServerDriverHost {
        public:
            MAKE_MOCK3(TrackedDeviceAdded, bool(const char*, vr::ETrackedDeviceClass, vr::ITrackedDeviceServerDriver*), override);
            MAKE_MOCK3(TrackedDevicePoseUpdated, void(uint32_t, const vr::DriverPose_t&, uint32_t), override);
            MAKE_MOCK1(VsyncEvent, void(double), override);
            MAKE_MOCK4(VendorSpecificEvent, void(uint32_t, vr::EVREventType, const vr::VREvent_Data_t&, double), override);
            MAKE_MOCK0(IsExiting, bool(), override);
            MAKE_MOCK2(PollNextEvent, bool(vr::VREvent_t*, uint32_t), override);
            MAKE_MOCK3(GetRawTrackedDevicePoses, void(float, vr::TrackedDevicePose_t*, uint32_t), override);
            MAKE_MOCK3(TrackedDeviceDisplayTransformUpdated, void(uint32_t, vr::HmdMatrix34_t, vr::HmdMatrix34_t), override);
            MAKE_MOCK4(RequestRestart, void(const char*, const char*, const char*, const char*), override);
        };
    }
}
