#include "utility/Test.hpp"

using namespace northstar::math::types;

ST::XRPose northstar::test::utils::DefaultValidPoseFromStructureSensor(
    const std::array<float, 16>& ReferenceRowMajorPositionTransform) {
    double dTimeStamp = 5;
    float v3fLinearVelocity[3] = { 1.f, 2.f, 3.f };
    float v3fAngularVelocity[3] = { 4.f, 5.f, 6.f };
    float v3fLinearAcceleration[3] = { 7.f, 8.f, 9.f };
    float v3fAngularAcceleration[3] = { 10.f, 11.f, 12.f };

    ST::XRPose Pose;
    Pose.timestamp = dTimeStamp;
    Pose.mostRecentAccelSampleTimestamp = dTimeStamp;
    Pose.mostRecentGyroSampleTimestamp = dTimeStamp;
    Pose.mostRecentTrackedFrameTimestamp = dTimeStamp;

    // ST::Matrix4 is Column Major storage 
    for (uint8_t r = 0; r < 4; r++)
    {
        for (uint8_t c = 0; c < 4; c++)
        {
            auto unRowMajorIndex = (r * 4) + c;
            auto unColMajorIndex = (c * 4) + r;
            Pose.matrix.m[unColMajorIndex] = 
                ReferenceRowMajorPositionTransform[unRowMajorIndex];
        }
    }

    Pose.linearVelocity[0] = v3fLinearVelocity[0];
    Pose.linearVelocity[1] = v3fLinearVelocity[1];
    Pose.linearVelocity[2] = v3fLinearVelocity[2];

    Pose.angularVelocity[0] = v3fAngularVelocity[0];
    Pose.angularVelocity[1] = v3fAngularVelocity[1];
    Pose.angularVelocity[2] = v3fAngularVelocity[2];

    Pose.linearAcceleration[0] = v3fLinearAcceleration[0];
    Pose.linearAcceleration[1] = v3fLinearAcceleration[1];
    Pose.linearAcceleration[2] = v3fLinearAcceleration[2];

    Pose.angularAcceleration[0] = v3fAngularAcceleration[0];
    Pose.angularAcceleration[1] = v3fAngularAcceleration[1];
    Pose.angularAcceleration[2] = v3fAngularAcceleration[2];

    return Pose;
}

// Note: For Driver Spaces reference WorldAdapter
vr::DriverPose_t northstar::test::utils::GenerateExpectedOpenVRPoseGivenStructureSensorPose(
    const ST::XRPose& ReferencePose, 
    const std::array<float, 16>& afReferenceRowMajorPositionTransform,
    double dReferenceCurrentUnixTimestamp) {

    AffineMatrix4d m4dTransform;
    m4dTransform.matrix() <<
        afReferenceRowMajorPositionTransform[0], afReferenceRowMajorPositionTransform[1], afReferenceRowMajorPositionTransform[2], afReferenceRowMajorPositionTransform[3],
        afReferenceRowMajorPositionTransform[4], afReferenceRowMajorPositionTransform[5], afReferenceRowMajorPositionTransform[6], afReferenceRowMajorPositionTransform[7],
        afReferenceRowMajorPositionTransform[8], afReferenceRowMajorPositionTransform[9], afReferenceRowMajorPositionTransform[10], afReferenceRowMajorPositionTransform[11],
        afReferenceRowMajorPositionTransform[12], afReferenceRowMajorPositionTransform[13], afReferenceRowMajorPositionTransform[14], afReferenceRowMajorPositionTransform[15];

    // x in ovr is to the right, x in structure space is to the left, the rest match up
    m4dTransform = m4dTransform.scale(Vector3d(-1, 1, 1));
    northstar::math::types::Quaterniond qdRotation;
    qdRotation = m4dTransform.rotation();

    vr::DriverPose_t ExpectedPose;
    ExpectedPose.poseTimeOffset = dReferenceCurrentUnixTimestamp - ReferencePose.timestamp;

    // TODO: figure out the correct one for this, identity for now
    ExpectedPose.qWorldFromDriverRotation.w = 1;
    ExpectedPose.qWorldFromDriverRotation.x = 0;
    ExpectedPose.qWorldFromDriverRotation.y = 0;
    ExpectedPose.qWorldFromDriverRotation.z = 0;

    // TODO: figure out the correct one for this, identity for now
    ExpectedPose.vecWorldFromDriverTranslation[0] = 0;
    ExpectedPose.vecWorldFromDriverTranslation[1] = 0;
    ExpectedPose.vecWorldFromDriverTranslation[2] = 0;

    /// TODO: Figure this out from config file, identity for now
    ExpectedPose.qDriverFromHeadRotation = vr::HmdQuaternion_t{ 1, 0, 0, 0 };
    ExpectedPose.vecDriverFromHeadTranslation[0] = 0;
    ExpectedPose.vecDriverFromHeadTranslation[1] = 0;
    ExpectedPose.vecDriverFromHeadTranslation[2] = 0;

    ExpectedPose.vecPosition[0] = m4dTransform.translation().x();
    ExpectedPose.vecPosition[1] = m4dTransform.translation().y();
    ExpectedPose.vecPosition[2] = m4dTransform.translation().z();

    ExpectedPose.vecVelocity[0] = -ReferencePose.linearVelocity[0];
    ExpectedPose.vecVelocity[1] = ReferencePose.linearVelocity[1];
    ExpectedPose.vecVelocity[2] = ReferencePose.linearVelocity[2];

    ExpectedPose.vecAcceleration[0] = -ReferencePose.linearAcceleration[0];
    ExpectedPose.vecAcceleration[1] = ReferencePose.linearAcceleration[1];
    ExpectedPose.vecAcceleration[2] = ReferencePose.linearAcceleration[2];

    ExpectedPose.qRotation.w = qdRotation.w();
    ExpectedPose.qRotation.x = qdRotation.x();
    ExpectedPose.qRotation.y = qdRotation.y();
    ExpectedPose.qRotation.z = qdRotation.z();

    ExpectedPose.vecAngularVelocity[0] = -ReferencePose.angularVelocity[0];
    ExpectedPose.vecAngularVelocity[1] = ReferencePose.angularVelocity[1];
    ExpectedPose.vecAngularVelocity[2] = ReferencePose.angularVelocity[2];

    ExpectedPose.vecAngularAcceleration[0] = -ReferencePose.angularAcceleration[0];
    ExpectedPose.vecAngularAcceleration[1] = ReferencePose.angularAcceleration[1];
    ExpectedPose.vecAngularAcceleration[2] = ReferencePose.angularAcceleration[2];

    ExpectedPose.result = vr::TrackingResult_Running_OK;
    ExpectedPose.poseIsValid = true;
    ExpectedPose.willDriftInYaw = false;
    // TODO: revisit after integration tests
    ExpectedPose.shouldApplyHeadModel = false;
    ExpectedPose.deviceIsConnected = true;

    return ExpectedPose;
}
