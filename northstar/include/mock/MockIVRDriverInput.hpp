#pragma once

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>
#include <openvr_driver.h>

namespace northstar {
    namespace openvr {
        class MockIVRDriverInput : public vr::IVRDriverInput {
            MAKE_MOCK3(CreateBooleanComponent, vr::EVRInputError(vr::PropertyContainerHandle_t, const char*, vr::VRInputComponentHandle_t*), override);
            MAKE_MOCK3(UpdateBooleanComponent, vr::EVRInputError(vr::VRInputComponentHandle_t, bool, double), override);
            MAKE_MOCK5(CreateScalarComponent, vr::EVRInputError(vr::PropertyContainerHandle_t, const char*, vr::VRInputComponentHandle_t*, vr::EVRScalarType, vr::EVRScalarUnits), override);
            MAKE_MOCK3(UpdateScalarComponent, vr::EVRInputError(vr::VRInputComponentHandle_t, float, double), override);
            MAKE_MOCK3(CreateHapticComponent, vr::EVRInputError(vr::PropertyContainerHandle_t, const char*, vr::VRInputComponentHandle_t*), override);
            MAKE_MOCK8(CreateSkeletonComponent, vr::EVRInputError(vr::PropertyContainerHandle_t, const char*, const char*, const char*, vr::EVRSkeletalTrackingLevel, const vr::VRBoneTransform_t*, uint32_t, vr::VRInputComponentHandle_t*), override);
            MAKE_MOCK4(UpdateSkeletonComponent, vr::EVRInputError(vr::VRInputComponentHandle_t, vr::EVRSkeletalMotionRange, const vr::VRBoneTransform_t*, uint32_t), override);
        };
    }
}