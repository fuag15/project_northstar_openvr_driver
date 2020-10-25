#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>
#include <memory>


#include "mock/MockILogger.hpp"
#include "mock/MockIEnvironmentSensor.hpp"
#include "mock/MockIVRSettings.hpp"
#include "mock/MockIVRServerDriverHost.hpp"
#include "mock/MockIMatrixFactory.hpp"
#include "mock/MockIVectorFactory.hpp"
#include "mock/MockIGeometry.hpp"
#include "mock/MockIOptics.hpp"
#include "mock/MockIVRProperties.hpp"
#include "mock/MockIHostProber.hpp"
#include "mock/MockISensorFrameCoordinator.hpp"
#include "utility/Test.hpp"
#include "driver/HMD.hpp"
#include "math/Types.hpp"

#include <ST/XRSession.h>
#include <ST/XRTypes.h>
#include <LeapC.h>

using trompeloeil::_;
using northstar::utility::MockILogger;
using northstar::driver::MockIEnvironmentSensor;
using northstar::driver::MockISensorFrameCoordinator;
using northstar::openvr::MockIVRSettings;
using northstar::openvr::MockIVRServerDriverHost;
using northstar::openvr::MockIVRProperties;
using northstar::utility::MockIHostProber;
using northstar::math::MockIVectorFactory;
using northstar::driver::MockIOptics;
using namespace northstar::test::utils;
using namespace northstar::math::types;

// TODO: this test suite leaks names into global namespace, setup logic should be in SCENARIO's
// this also has a side effect of making the order of test matter, yuck
TEST_SUITE("HMD") {
    auto pMockIVRSettings = std::make_shared<MockIVRSettings>();
    auto pMockIVRProperties = std::make_shared<MockIVRProperties>();
    auto pMockIVRServerDriverHost = std::make_shared<MockIVRServerDriverHost>();
    auto pMockLogger = std::make_shared<MockILogger>();
    auto pMockEnvironmentSensor = std::make_shared<MockIEnvironmentSensor>();
    auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
    auto pMockOptics = std::make_shared<MockIOptics>();
    auto pMockHostProber = std::make_shared<MockIHostProber>();
    auto pMockSensorFrameCoordinator = std::make_shared<MockISensorFrameCoordinator>();
    float fFakeGetBool = false;
    float fFakeGetFloat = 1.0f;
    uint32_t iFakeGetInt = 1;

    ALLOW_CALL(*pMockSensorFrameCoordinator, SubmitOpenVRHeadsetPose(_));
    ALLOW_CALL(*pMockIVRSettings, GetBool(_, _, _)).RETURN(fFakeGetBool);
    ALLOW_CALL(*pMockIVRSettings, GetFloat(_, _, _)).RETURN(fFakeGetFloat);
    ALLOW_CALL(*pMockIVRSettings, GetInt32(_, _, _)).RETURN(iFakeGetInt);
    ALLOW_CALL(*pMockIVRSettings, SetFloat(_, _, _, _));
    ALLOW_CALL(*pMockIVRProperties, SetFloatProperty(_, _, _)).RETURN(vr::ETrackedPropertyError::TrackedProp_Success);
    ALLOW_CALL(*pMockIVRProperties, SetStringProperty(_, _, _)).RETURN(vr::ETrackedPropertyError::TrackedProp_Success);
    ALLOW_CALL(*pMockIVRProperties, SetBoolProperty(_, _, _)).RETURN(vr::ETrackedPropertyError::TrackedProp_Success);
    ALLOW_CALL(*pMockIVRProperties, SetInt32Property(_, _, _)).RETURN(vr::ETrackedPropertyError::TrackedProp_Success);
    ALLOW_CALL(*pMockIVRProperties, SetUint64Property(_, _, _)).RETURN(vr::ETrackedPropertyError::TrackedProp_Success);
    ALLOW_CALL(*pMockIVRProperties, TrackedDeviceToPropertyContainer(_)).RETURN(1);
    auto Subject = northstar::driver::CHMD(
        pMockIVRSettings.get(), 
        pMockIVRServerDriverHost.get(),
        pMockHostProber,
        pMockIVRProperties,
        pMockEnvironmentSensor, 
        pMockVectorFactory,
        pMockOptics,
        pMockSensorFrameCoordinator,
        pMockLogger);

    // TODO: Test
    SCENARIO("Must load relevant configuration data") {
        WHEN("Initialized") {
            THEN("it should hold the right value for screen start X") {}
        }
    }

    SCENARIO("Must simulate all relevant things during update loops") {
        WHEN("Runframe is called") {
            GIVEN("There is an active simulation") {
                Subject.Activate(100);
                THEN("Run frame sets an updated pose") {
                    REQUIRE_CALL(*pMockEnvironmentSensor,
                        GetPose(_, _))
                        .SIDE_EFFECT(_2 = northstar::driver::IEnvironmentSensor::EPoseRetrievalError::SessionStartFailed)
                        .RETURN(false);

                    REQUIRE_CALL(*pMockIVRServerDriverHost,
                        TrackedDevicePoseUpdated(_, _, _));

                    Subject.RunFrame();
                }
                Subject.Deactivate();
            }

            GIVEN("There is not an active simulation") {
                THEN("Run frame does not set an updated pose") {
                    Subject.RunFrame();
                }
            }
        }
    }

    SCENARIO("must signal openvr with the apropriate information") {
        WHEN("Activate is called") {
            auto result = Subject.Activate(100);
            THEN("No error is returned") {
                CHECK(result == vr::VRInitError_None);
            }
        }
        WHEN("Deactivate is called") {}
        WHEN("GetComponent is called") {
            GIVEN("The component requested is the display component") {
                auto result = Subject.GetComponent(vr::IVRDisplayComponent_Version);
                THEN("It returns a cast pointer to iteslf") {
                    CHECK(result != nullptr);
                }
            }

            GIVEN("The component requested is not the display component") {
                auto result = Subject.GetComponent("zz.ly");
                THEN("It returns a nullptr") {
                    CHECK(result == nullptr);
                }
            }
        }

        WHEN("IsDisplayOnDesktop is called") {
            auto result = Subject.IsDisplayOnDesktop();
            THEN("The right value is returned") {
                CHECK(result == true);
            }
        }

        WHEN("IsDisplayRealDisplay is called") {
            auto result = Subject.IsDisplayRealDisplay();
            THEN("The right value is returned") {
                CHECK(result == true);
            }
        }

        WHEN("GetWindowBounds is called") {
            int32_t pnX = 0, 
                pnY = 0; 

            uint32_t pnWidth = 0, 
                pnHeight = 0;

            Subject.GetWindowBounds(&pnX, &pnY, &pnWidth, &pnHeight);
            THEN("The values are filled out") {
                CHECK(pnX == iFakeGetInt);
                CHECK(pnY == iFakeGetInt);
                CHECK(pnWidth == iFakeGetInt);
                CHECK(pnHeight == iFakeGetInt);
            }
        }

        WHEN("GetRecommendedRenderTargetSize is called") {
            uint32_t pnWidth = 0, 
                pnHeight = 0;

            Subject.GetRecommendedRenderTargetSize(&pnWidth, &pnHeight);
            THEN("The values are filled out") {
                CHECK(pnWidth == iFakeGetInt);
                CHECK(pnHeight == iFakeGetInt);
            }
        }

        GIVEN("The eye requested is the left eye") {
            auto eye = vr::EVREye::Eye_Left;
            WHEN("GetEyeOutputViewport is called") {
                uint32_t  pnX = 0,
                    pnY = 0,
                    pnWidth = 0, 
                    pnHeight = 0;

                Subject.GetEyeOutputViewport(eye, &pnX, &pnY, &pnWidth, &pnHeight);
                THEN("The values are filled out") {
                    CHECK(pnX == 0);
                    CHECK(pnY == 0);
                    CHECK(pnWidth == iFakeGetInt / 2);
                    CHECK(pnHeight == iFakeGetInt);
                }
            }

            WHEN("GetProjectionRaw is called") {
                THEN("The right result is given") {
                    Vector4d v4dGiven(1, 2, 3, 4);
                    REQUIRE_CALL(*pMockOptics,
                        GetEyeProjectionLRTB(eye))
                        .RETURN(v4dGiven);

                    float pfLeft = 0,
                        pfRight = 0,
                        pfTop = 0,
                        pfBottom = 0;

                    Subject.GetProjectionRaw(eye, &pfLeft, &pfRight, &pfTop, &pfBottom);
                    CHECK(pfLeft == v4dGiven.x());
                    CHECK(pfRight == v4dGiven.y());
                    CHECK(pfTop == v4dGiven.z());
                    CHECK(pfBottom == v4dGiven.w());
                }
            }
        }

        GIVEN("The eye requested is the right eye") {
            auto eye = vr::EVREye::Eye_Right;
            WHEN("GetEyeOutputViewport is called") {
                uint32_t  pnX = 0,
                    pnY = 0,
                    pnWidth = 0, 
                    pnHeight = 0;

                Subject.GetEyeOutputViewport(eye, &pnX, &pnY, &pnWidth, &pnHeight);
                THEN("The values are filled out") {
                    CHECK(pnX == iFakeGetInt / 2);
                    CHECK(pnY == 0);
                    CHECK(pnWidth == iFakeGetInt / 2);
                    CHECK(pnHeight == iFakeGetInt);
                }
            }

            WHEN("GetProjectionRaw is called") {
                THEN("The right result is given") {
                    Vector4d v4dGiven(1, 2, 3, 4);
                    REQUIRE_CALL(*pMockOptics,
                        GetEyeProjectionLRTB(eye))
                        .RETURN(v4dGiven);

                    float pfLeft = 0,
                        pfRight = 0,
                        pfTop = 0,
                        pfBottom = 0;

                    Subject.GetProjectionRaw(eye, &pfLeft, &pfRight, &pfTop, &pfBottom);
                    CHECK(pfLeft == v4dGiven.x());
                    CHECK(pfRight == v4dGiven.y());
                    CHECK(pfTop == v4dGiven.z());
                    CHECK(pfBottom == v4dGiven.w());
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

    SCENARIO("Must compute UV warps when asked") {
        GIVEN("The eye requested is the left eye") {
            auto eye = vr::EVREye::Eye_Left;
            WHEN("ComputeDistortion is called") {
                THEN("It returns the distortion from Optics") {
                    Vector2d pMockWarp(0.5, 0.5);
                    REQUIRE_CALL(*pMockOptics,
                        EyeUVToScreenUV(eye, _))
                        .RETURN(pMockWarp);

                    REQUIRE_CALL(*pMockVectorFactory,
                        V2DFromArray(_))
                        .RETURN(Vector2d(_1[0], _1[1]));
                    
                    auto result = Subject.ComputeDistortion(eye, 0.1f, 0.1f);
                    CompareIndexed(pMockWarp, result.rfRed, 0, 1);
                    CompareIndexed(pMockWarp, result.rfGreen, 0, 1);
                    CompareIndexed(pMockWarp, result.rfBlue, 0, 1);
                }
            }
        }

        GIVEN("The eye requested is the right eye") {
            auto eye = vr::EVREye::Eye_Right;
            WHEN("ComputeDistortion is called") {
                THEN("It returns the distortion from Optics") {
                    Vector2d pMockWarp(0.5, 0.5);
                    REQUIRE_CALL(*pMockOptics,
                        EyeUVToScreenUV(eye, _))
                        .RETURN(pMockWarp);

                    REQUIRE_CALL(*pMockVectorFactory,
                        V2DFromArray(_))
                        .RETURN(Vector2d(_1[0], _1[1]));

                    auto result = Subject.ComputeDistortion(eye, 0.1f, 0.1f);
                    CompareIndexed(pMockWarp, result.rfRed, 0, 1);
                    CompareIndexed(pMockWarp, result.rfGreen, 0, 1);
                    CompareIndexed(pMockWarp, result.rfBlue, 0, 1);
                }
            }
        }
    }

    SCENARIO("HMD must provide Initial pose information to OpenVR") {
        GIVEN("StructureSensor failed to start") {
            REQUIRE_CALL(*pMockEnvironmentSensor,
                GetPose(_, _))
                .SIDE_EFFECT(_2 = northstar::driver::IEnvironmentSensor::EPoseRetrievalError::SessionStartFailed)
                .RETURN(false);

            WHEN("Get Pose is called") {
                auto result = Subject.GetPose();

                THEN("The pose returned indicates the correct state to openvr") {
                    CHECK(result.poseIsValid == false);
                    CHECK(result.deviceIsConnected == false);
                }
            }
        }

        GIVEN("StructureSensor started") {
            GIVEN("Structure Sensor is Running") {
                GIVEN("Structure Sensor is initializing") {
                    REQUIRE_CALL(*pMockEnvironmentSensor,
                        GetPose(_, _))
                        .SIDE_EFFECT(_2 = northstar::driver::IEnvironmentSensor::EPoseRetrievalError::SessionInitializing)
                        .RETURN(false);

                    WHEN("Get pose is called") {
                        auto result = Subject.GetPose();

                        THEN("The pose returned indicates this to OpenVR") {
                            CHECK(result.poseIsValid == false);
                            CHECK(result.deviceIsConnected == false);
                        }
                    }
                }

                GIVEN("Structure Sensor has initialized") {
                    GIVEN("Structure Sensor Has an invalid pose") {
                        REQUIRE_CALL(*pMockEnvironmentSensor,
                            GetPose(_, _))
                            .SIDE_EFFECT(_2 = northstar::driver::IEnvironmentSensor::EPoseRetrievalError::InvalidPoseRecieved)
                            .RETURN(false);

                        WHEN("Get pose is called") {
                            auto result = Subject.GetPose();

                            THEN("The pose returned indicates this to OpenVR") {
                                CHECK(result.poseIsValid == false);
                                CHECK(result.deviceIsConnected == false);
                            }
                        }
                    }

                    //TODO: test
                    GIVEN("Structure Sensor Has an valid pose") {
                        WHEN("Get pose is called") {
                            THEN("An equivalent pose is returned, with relevant data translated from StructureSensor space to OpenVR space") {}
                        }
                    }
                }
            }

            GIVEN("Structure Sensor is not Running") {
                REQUIRE_CALL(*pMockEnvironmentSensor,
                    GetPose(_, _))
                    .SIDE_EFFECT(_2 = northstar::driver::IEnvironmentSensor::EPoseRetrievalError::SessionNotRunning)
                    .RETURN(false);

                WHEN("Get pose is called") {
                    auto result = Subject.GetPose();

                    THEN("The pose returned indicates this to OpenVR") {
                        CHECK(result.poseIsValid == false);
                        CHECK(result.deviceIsConnected == false);
                    }
                }
            }
        }
    }
}
