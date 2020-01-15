#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "driver/Controller.hpp"

#include "mock/MockISensorFrameCoordinator.hpp"
#include "mock/MockIWorldAdapter.hpp"
#include "mock/MockIVectorFactory.hpp"
#include "mock/MockILogger.hpp"
#include "mock/MockIVRProperties.hpp"
#include "mock/MockIVRSettings.hpp"
#include "mock/MockIVRServerDriverHost.hpp"
#include "mock/MockIVRDriverInput.hpp"
#include "mock/MockISkeletalAdapter.hpp"

#include "driver/Types.hpp"

using trompeloeil::_;

using namespace northstar::driver::types;
using namespace northstar::math::types;
using northstar::driver::MockISensorFrameCoordinator;
using northstar::math::MockIWorldAdapter;
using northstar::math::MockIVectorFactory;
using northstar::math::MockISkeletalAdapter;
using northstar::utility::MockILogger;
using northstar::openvr::MockIVRProperties;
using northstar::openvr::MockIVRSettings;
using northstar::openvr::MockIVRServerDriverHost;
using northstar::openvr::MockIVRDriverInput;

TEST_SUITE("CController") {
    // TODO: polish pass to test this
    SCENARIO("must load relevant configuration data") {
        WHEN("Initialized") {
            THEN("It should hold the right value for screen start X") {}
        }
    }

    // TODO: Test this
    SCENARIO("Must simulate all relevant things during update loops") {
        WHEN("Run frame is called") {
            GIVEN("There is an active simulation") {
                THEN("An apropriate pose should be provided") {}
                THEN("Appropriate input events should be provided") {}
                THEN("Appropriate input skeletal data should be provided") {}
            }

            GIVEN("There is not an active simulation") {
                THEN("No pose should be provided") {}
                THEN("No input events should be provided") {}
                THEN("No input skeletal data should be provided") {}
            }
        }
    }

    SCENARIO("must signal openvr with the apropriate information") {
        auto spMockSensorFrameCoordinator = std::make_shared<MockISensorFrameCoordinator>();
        auto spMockWorldAdapter = std::make_shared<MockIWorldAdapter>();
        auto spMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto spMockSkeletalAdapter = std::make_shared<MockISkeletalAdapter>();
        auto spMockLogger = std::make_shared<MockILogger>();
        auto spMockVRProperties = std::make_shared<MockIVRProperties>();
        auto spMockVRSettings = std::make_shared<MockIVRSettings>();
        auto spMockVRServerDriverHost = std::make_shared<MockIVRServerDriverHost>();
        auto spMockVRDriverInput = std::make_shared<MockIVRDriverInput>();

        ALLOW_CALL(*spMockVRSettings, GetFloat(_, _, _)).RETURN(1.0);
        ALLOW_CALL(*spMockVRProperties, SetStringProperty(_, _, _)).RETURN(vr::ETrackedPropertyError::TrackedProp_Success);
        ALLOW_CALL(*spMockVRProperties, SetInt32Property(_, _, _)).RETURN(vr::ETrackedPropertyError::TrackedProp_Success);
        ALLOW_CALL(*spMockVRProperties, SetUint64Property(_, _, _)).RETURN(vr::ETrackedPropertyError::TrackedProp_Success);
        ALLOW_CALL(*spMockVRProperties, TrackedDeviceToPropertyContainer(_)).RETURN(1);
        ALLOW_CALL(*spMockVRDriverInput, CreateBooleanComponent(_, _, _)).RETURN(vr::VRInputError_None);
        ALLOW_CALL(*spMockVRDriverInput, UpdateBooleanComponent(_, _, _)).RETURN(vr::VRInputError_None);
        ALLOW_CALL(*spMockVRDriverInput, CreateSkeletonComponent(_, _, _, _, _, _, _, _)).RETURN(vr::VRInputError_None);
        ALLOW_CALL(*spMockVRDriverInput, UpdateSkeletonComponent(_, _, _, _)).RETURN(vr::VRInputError_None);
        ALLOW_CALL(*spMockVectorFactory, V3DFromArray(_)).RETURN(Vector3d(_1[0], _1[1], _1[2]));
        ALLOW_CALL(*spMockWorldAdapter, FromUnityQuaternionToOpenVRQuaternion(_)).RETURN(Quaterniond(_1.w(), -_1.x(), -_1.y(), _1.z()));
        ALLOW_CALL(*spMockWorldAdapter, FromUnityPositionToOpenVRPosition(_)).RETURN(Vector3d(_1.x(), _1.y(), -_1.z()));

        northstar::driver::CController Subject(
            spMockSensorFrameCoordinator,
            spMockWorldAdapter,
            spMockVectorFactory,
            spMockSkeletalAdapter,
            spMockLogger,
            spMockVRProperties,
            spMockVRSettings.get(),
            spMockVRServerDriverHost.get(),
            spMockVRDriverInput.get(),
            EHand::Left);

        WHEN("Activate is called") {
            auto result = Subject.Activate(100);
            THEN("No error is returned") {
                CHECK(result == vr::VRInitError_None);
            }
        }
        WHEN("Deactivate is called") {}
        WHEN("GetComponent is called") {
            GIVEN("The component requested is anything") {
                auto result = Subject.GetComponent("zz.ly");
                THEN("It returns a nullptr") {
                    CHECK(result == nullptr);
                }
            }
        }

        WHEN("DebugRequest is called") {
            GIVEN("The buffer size is not 0") {
                char supplied[1] = { 'c' };
                Subject.DebugRequest("dummy", supplied, 1);
                THEN("The response is set to 0") {
                    CHECK(supplied[0] == 0);
                }
            }

            GIVEN("The buffer size is 0") {
                char supplied[1] = { 'c' };
                Subject.DebugRequest("dummy", supplied, 0);
                THEN("The response is left alone") {
                    CHECK(supplied[0] == 'c');
                }
            }
        }

        WHEN("GetSerialNumber is called") {}
    }

    // TODO: test this
    SCENARIO("Must provide pose and input information to OpenVR") {
        WHEN("get pose is called in a variety of conditions") {
            THEN("it gives the right pose") {}
        }
    }
}
