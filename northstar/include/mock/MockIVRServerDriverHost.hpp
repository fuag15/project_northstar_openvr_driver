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
            MAKE_MOCK4(RequestRestart, void(const char*, const char*, const char*, const char*), override);
			MAKE_MOCK2(GetFrameTimings, uint32_t(vr::Compositor_FrameTiming*, uint32_t), override);
            MAKE_MOCK3(SetDisplayEyeToHead, void(uint32_t, const vr::HmdMatrix34_t&, const vr::HmdMatrix34_t&), override);
            MAKE_MOCK3(SetDisplayProjectionRaw, void(uint32_t, const vr::HmdRect2_t&, const vr::HmdRect2_t&), override);
            MAKE_MOCK3(SetRecommendedRenderTargetSize, void(uint32_t, uint32_t, uint32_t), override);
        };
    }
}
