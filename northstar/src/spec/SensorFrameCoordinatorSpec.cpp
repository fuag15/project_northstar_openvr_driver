#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include <memory>

#include "driver/SensorFrameCoordinator.hpp"
#include "driver/ILeapMotion.hpp"
#include "driver/Types.hpp"
#include "mock/MockILeapMotion.hpp"
#include "mock/MockILogger.hpp"

using northstar::driver::MockILeapMotion;
using northstar::utility::MockILogger;
using northstar::driver::ILeapMotion;
using namespace northstar::driver::types;

TEST_SUITE("SensorFrameCoordinator") {
    SCENARIO("must coordinate sensor frames") {
        auto pMockLeapMotion = std::make_shared<MockILeapMotion>();
        auto pMockLogger = std::make_shared<MockILogger>();
        northstar::driver::CSensorFrameCoordinator Subject(
            pMockLeapMotion, pMockLogger);

        GIVEN("the instance has no data") {
            WHEN("asked for hand pose") {
                auto result = Subject.GetLeapHandPose(EHand::Left);
                THEN("it returns nullopt") {
                    CHECK(result == std::nullopt);
                }
            }

            WHEN("asked for hmd pose") {
                auto result = Subject.GetOpenVRHeadPose();
                THEN("it returns nullopt") {
                    CHECK(result == std::nullopt);
                }
            }
        }

        GIVEN("The instance has head data") {
            vr::DriverPose_t sFakeDriverPose = { 0 };
            double dFakeTimeOffset = 15;
            LEAP_HAND sFakeLeftHand = { 0 };
            sFakeLeftHand.type = eLeapHandType_Left;
            sFakeLeftHand.confidence = 1.0;
            LEAP_HAND sFakeRightHand = { 0 };
            sFakeRightHand.type = eLeapHandType_Right;
            sFakeRightHand.confidence = 2.0;
            sFakeDriverPose.poseTimeOffset = dFakeTimeOffset;
            Subject.SubmitOpenVRHeadsetPose(sFakeDriverPose);
            WHEN("asked for a head pose") {
                auto result = Subject.GetOpenVRHeadPose();
                THEN("it returns that data") {
                    CHECK(result.has_value() == true);
                    if (result)
                        CHECK(result->poseTimeOffset == sFakeDriverPose.poseTimeOffset);
                }
            }

            GIVEN("Leap motion is not availalble") {
                REQUIRE_CALL(*pMockLeapMotion, GetFrame())
                    .RETURN(std::nullopt);

                WHEN("asked for hand pose") {
                    auto result = Subject.GetLeapHandPose(EHand::Left);
                    THEN("it returns nullopt") {
                        CHECK(result == std::nullopt);
                    }
                }
            }

            GIVEN("leap motion is available for only the left hand") {
                LEAP_TRACKING_EVENT sFakeLeapTrackingEvent = { 0 };
                LEAP_HAND asFakeHands[1] = { sFakeLeftHand };
                sFakeLeapTrackingEvent.nHands = 1;
                sFakeLeapTrackingEvent.pHands = asFakeHands;
                ALLOW_CALL(*pMockLeapMotion, GetFrame())
                    .RETURN(sFakeLeapTrackingEvent);

                WHEN("asked for the right hand") {
                    auto result = Subject.GetLeapHandPose(EHand::Right);
                    THEN("it returns nullopt") {
                        CHECK(result == std::nullopt);
                    }
                }

                WHEN("asked for the left hand") {
                    auto result = Subject.GetLeapHandPose(EHand::Left);
                    THEN("it returns the left hand") {
                        CHECK(result.has_value() == true);
                        if (result)
                            CHECK(result->confidence == sFakeLeftHand.confidence);
                    }
                }
            }

            GIVEN("leap motion is available for both hands") {
                LEAP_TRACKING_EVENT sFakeLeapTrackingEvent = { 0 };
                LEAP_HAND asFakeHands[2] = { sFakeLeftHand, sFakeRightHand };
                sFakeLeapTrackingEvent.nHands = 2;
                sFakeLeapTrackingEvent.pHands = asFakeHands;
                ALLOW_CALL(*pMockLeapMotion, GetFrame())
                    .RETURN(sFakeLeapTrackingEvent);

                WHEN("asked for the right hand") {
                    auto result = Subject.GetLeapHandPose(EHand::Right);
                    THEN("it returns the right hand") {
                        CHECK(result.has_value() == true);
                        if (result)
                            CHECK(result->confidence == sFakeRightHand.confidence);
                    }
                }

                WHEN("asked for the left hand") {
                    auto result = Subject.GetLeapHandPose(EHand::Left);
                    THEN("it returns the left hand") {
                        CHECK(result.has_value() == true);
                        if (result)
                            CHECK(result->confidence == sFakeLeftHand.confidence);
                    }
                }

                WHEN("frame state is reset") {
                    Subject.ResetFrameState();
                    auto resultHMD = Subject.GetOpenVRHeadPose();
                    auto resultRightHand = Subject.GetLeapHandPose(EHand::Right);
                    auto resultLeftHand = Subject.GetLeapHandPose(EHand::Left);
                    THEN("hand and hmd data is invalidated") {
                        CHECK(resultHMD == std::nullopt);
                        CHECK(resultRightHand == std::nullopt);
                        CHECK(resultLeftHand == std::nullopt);
                    }
                }
            }
        }
    }
}