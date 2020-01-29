#include "math/SkeletalAdapter.hpp"

using namespace northstar::driver::types;
using namespace northstar::math::types;

northstar::math::CSkeletalAdapter::CSkeletalAdapter(
    std::shared_ptr<IVectorFactory> pVectorFactory, 
    std::shared_ptr<IMatrixFactory> pMatrixFactory) {
    m_pVectorFactory = pVectorFactory;
    m_pMatrixFactory = pMatrixFactory;
}

// TODO: figure out correct bone space via testing, cant find docs on it
// TODO: root transform?
// TODO: clean up
/*
Leap provides a palm centric approach to bones with a front and back

For the most part front is considered further away from the heart,
back considered closer.

For the wrist hoever, this is inverted.

This means that for OVR space, which has it's root at the "front" of the wrist.
The wrist will use the front not the back and invert its orientation to be relative to the body,
as OVR expects.

For finger tip bones, the front end of the distal phalanges is used with no change to orientation.

For auxilary bones the back end of the distal phalanges is used.

Leap motion provides a 0 length metacarpal for the thumb, the OVR skeleton does not so this 
bone is skipped.

Following medical names as well as their relevant bones, aside from "arm" Leap uses the terms expected

Metacarpal, Poximal, Intermediate, distal phalanges.

The provided struct gives palm information that could be used to define a "root" but it is unknown how
OVR expects such things and will need to be figured out through trial and error as the docs don't seem
very specific in this area.

A leap hand frame is organized by palm, arm, and digits, the digits have aliases that more concretely describe them
where a digit is a full 4 set of bones.

This function populates the given arrray respecting the order of the enumeration given for bone indicies.

OpenVR is the best source of truth on the skeleton but in general they have meta bones for the base of the metacarpals
and root is considered the end of the wrist. Bones are defined by their "back" end in Leap terms.

The space is said to be global leap space but needs to be investigated further as to relative rotations or absolute and
what OpenVR expects

Investigation: Root is from skeletal root, positions should always have w component of 1, start root at 0,0,0,1...1,0,0,0
bones need to be in parent relative space using OpenVR / XR / Vulkan coordinate space, make helper function for this
*/
void northstar::math::CSkeletalAdapter::FromLeapMotionHandToOVRBonePoseArray(
    const vr::DriverPose_t& sPose,
    const AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
    const AffineMatrix4d& m4dFromHMDToWorldSpace,
    const LEAP_HAND& sLeapHand, 
    std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>& saOpenVRBoneTargets) const {
    AffineMatrix4d m4dWorldToHandRelativeSpace = m_pMatrixFactory->FromTraslationAndRotation(
        m_pVectorFactory->V3DFromArray({ sPose.vecPosition[0], sPose.vecPosition[1], sPose.vecPosition[2] }),
        Quaterniond(sPose.qRotation.w, sPose.qRotation.x, sPose.qRotation.y, sPose.qRotation.z)).inverse();

    // Wrist as root
    auto v4dRootPosition = m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace * GetTranslationSensitiveV4DFromLeapVectorPosition(sLeapHand.arm.prev_joint);
    auto qdRootOrientation = (m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace * GetQuaterniondFromLeapQuaternionOrientation(sLeapHand.arm.rotation)).inverse();
    if (x_dIgnorePalmOffset) {
        m4dWorldToHandRelativeSpace = m_pMatrixFactory->FromTraslationAndRotation(
            m_pVectorFactory->V3DXYZFromV4D(v4dRootPosition),
            Quaterniond(qdRootOrientation.rotation())).inverse();
    }

    auto v4dPalmRelativeRootPosition = m4dWorldToHandRelativeSpace * v4dRootPosition;
    auto qdPalmRelativeRootOrientation = m4dWorldToHandRelativeSpace * qdRootOrientation;
    WritePositionAndOrientationIntoVRBonePose(
        saOpenVRBoneTargets[static_cast<int32_t>(EHandeSkeletonBone::Root)],
        v4dPalmRelativeRootPosition,
        Quaterniond(qdPalmRelativeRootOrientation.rotation()));

    AffineMatrix4d v4dWorldToRootBoneRelativeSpace = m_pMatrixFactory->FromTraslationAndRotation(
        m_pVectorFactory->V3DXYZFromV4D(v4dRootPosition),
        Quaterniond(qdRootOrientation.rotation())).inverse();

    WriteConvertedDigitToTargetOVRBoneIndicies(
        v4dWorldToRootBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.thumb,
        {EHandeSkeletonBone::Wrist, EHandeSkeletonBone::Thumb0, EHandeSkeletonBone::Thumb1, EHandeSkeletonBone::Thumb2, EHandeSkeletonBone::Thumb3},
        EHandeSkeletonBone::Aux_Thumb,
        saOpenVRBoneTargets);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        v4dWorldToRootBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.index,
        {EHandeSkeletonBone::IndexFinger0, EHandeSkeletonBone::IndexFinger1, EHandeSkeletonBone::IndexFinger2, EHandeSkeletonBone::IndexFinger3, EHandeSkeletonBone::IndexFinger4},
        EHandeSkeletonBone::Aux_IndexFinger,
        saOpenVRBoneTargets);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        v4dWorldToRootBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.middle,
        {EHandeSkeletonBone::MiddleFinger0, EHandeSkeletonBone::MiddleFinger1, EHandeSkeletonBone::MiddleFinger2, EHandeSkeletonBone::MiddleFinger3, EHandeSkeletonBone::MiddleFinger4},
        EHandeSkeletonBone::Aux_MiddleFinger,
        saOpenVRBoneTargets);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        v4dWorldToRootBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.ring,
        {EHandeSkeletonBone::RingFinger0, EHandeSkeletonBone::RingFinger1, EHandeSkeletonBone::RingFinger2, EHandeSkeletonBone::RingFinger3, EHandeSkeletonBone::RingFinger4},
        EHandeSkeletonBone::Aux_RingFinger,
        saOpenVRBoneTargets);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        v4dWorldToRootBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.pinky,
        {EHandeSkeletonBone::PinkyFinger0, EHandeSkeletonBone::PinkyFinger1, EHandeSkeletonBone::PinkyFinger2, EHandeSkeletonBone::PinkyFinger3, EHandeSkeletonBone::PinkyFinger4},
        EHandeSkeletonBone::Aux_PinkyFinger,
        saOpenVRBoneTargets);
}

// TODO: Clean this up
/*
it is assumed that the index order is in the order of leap data

Metacarpal, Poximal, Intermediate, distal phalanges.

with the fifth being the end of the Digit
*/
void northstar::math::CSkeletalAdapter::WriteConvertedDigitToTargetOVRBoneIndicies(
    const types::AffineMatrix4d& m4dFromWorldToRootBoneRelativeSpace,
    const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
    const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
    const LEAP_DIGIT& sLeapDigit,
    const std::array<EHandeSkeletonBone, 5>& aeDestinationIndicies,
    const EHandeSkeletonBone& eAuxBoneIndex,
    std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>& saOpenVRBoneTargets) inline const {
    AffineMatrix4d m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dFromWorldToRootBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.metacarpal.prev_joint,
        sLeapDigit.metacarpal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[0])]);

    m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dCurrentWorldToParentRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.proximal.prev_joint,
        sLeapDigit.proximal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[1])]);

    m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dCurrentWorldToParentRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.intermediate.prev_joint,
        sLeapDigit.intermediate.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[2])]);

    m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dCurrentWorldToParentRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.distal.prev_joint,
        sLeapDigit.distal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[3])]);

    m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dCurrentWorldToParentRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.distal.next_joint,
        sLeapDigit.distal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[4])]);

    ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dFromWorldToRootBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.distal.prev_joint,
        sLeapDigit.distal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(eAuxBoneIndex)]);
}

//TODO: This is the wip of the moment
AffineMatrix4d northstar::math::CSkeletalAdapter::ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
    const types::AffineMatrix4d& m4dFromWorldSpaceToParentRelativeSpace,
    const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
    const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
    const LEAP_VECTOR& sLeapPosition,
    const LEAP_QUATERNION& sLeapOrientation,
    vr::VRBoneTransform_t& sVRBoneTransform) inline const {
    const auto m4dFromLeapSensorToWorldSpace = m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace;
    auto v4dWorldSpaceRootPosition = m4dFromLeapSensorToWorldSpace * GetTranslationSensitiveV4DFromLeapVectorPosition(sLeapPosition);
    auto qdWorldSpaceRootOrientation = m4dFromLeapSensorToWorldSpace * m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace * GetQuaterniondFromLeapQuaternionOrientation(sLeapOrientation);
    auto qdParentSpaceRootOrientation = m4dFromWorldSpaceToParentRelativeSpace * qdWorldSpaceRootOrientation;
    WritePositionAndOrientationIntoVRBonePose(
        sVRBoneTransform, 
        m4dFromWorldSpaceToParentRelativeSpace * v4dWorldSpaceRootPosition, 
        Quaterniond(qdParentSpaceRootOrientation.rotation()));

    return m_pMatrixFactory->FromTraslationAndRotation(
        m_pVectorFactory->V3DXYZFromV4D(v4dWorldSpaceRootPosition),
        Quaterniond(qdWorldSpaceRootOrientation.rotation())).inverse();
}

Vector4d northstar::math::CSkeletalAdapter::GetTranslationSensitiveV4DFromLeapVectorPosition(const LEAP_VECTOR& sPosition) inline const {
    return m_pVectorFactory->V4DFromXYZWArray({
        sPosition.x,
        sPosition.y,
        sPosition.z,
        1.0 });
}

Quaterniond northstar::math::CSkeletalAdapter::GetQuaterniondFromLeapQuaternionOrientation(const LEAP_QUATERNION& sOrientation) inline const {
    return Quaterniond(sOrientation.w, sOrientation.x, sOrientation.y, sOrientation.z);
}

void northstar::math::CSkeletalAdapter::WritePositionAndOrientationIntoVRBonePose(
    vr::VRBoneTransform_t& sVRBoneTransform, 
    const types::Vector4d& v4dPosition, 
    const types::Quaterniond& qdOrientation) const {
    sVRBoneTransform.position = { 
        static_cast<float>(v4dPosition.x()), 
        static_cast<float>(v4dPosition.y()), 
        static_cast<float>(v4dPosition.z()), 
        1.f };

    sVRBoneTransform.orientation = { 
        static_cast<float>(qdOrientation.w()), 
        static_cast<float>(qdOrientation.x()), 
        static_cast<float>(qdOrientation.y()), 
        static_cast<float>(qdOrientation.z()) };
}
