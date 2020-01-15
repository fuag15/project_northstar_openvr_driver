#include "math/SkeletalAdapter.hpp"

using namespace northstar::driver::types;
using namespace northstar::math::types;

northstar::math::CSkeletalAdapter::CSkeletalAdapter(std::shared_ptr<IVectorFactory> pVectorFactory) {
    m_pVectorFactory = pVectorFactory;
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
*/
void northstar::math::CSkeletalAdapter::FromLeapMotionHandToOVRBonePoseArray(
    const AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
    const AffineMatrix4d& m4dFromHMDToWorldSpace,
    const LEAP_HAND& sLeapHand, 
    std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>& saOpenVRBoneTargets) const {
    // Wrist as root
    auto v4dRootPosition = m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace * GetTranslationSensitiveV4DFromLeapVectorPosition(sLeapHand.arm.prev_joint);
    auto qdRootOrientation = (m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace * GetQuaterniondFromLeapQuaternionOrientation(sLeapHand.arm.rotation)).inverse();
    WritePositionAndOrientationIntoVRBonePose(
        saOpenVRBoneTargets[static_cast<int32_t>(EHandeSkeletonBone::Root)],
        v4dRootPosition,
        Quaterniond(qdRootOrientation.rotation()));

    WriteConvertedDigitToTargetOVRBoneIndicies(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.thumb,
        {EHandeSkeletonBone::Wrist, EHandeSkeletonBone::Thumb0, EHandeSkeletonBone::Thumb1, EHandeSkeletonBone::Thumb2, EHandeSkeletonBone::Thumb3},
        EHandeSkeletonBone::Aux_Thumb,
        saOpenVRBoneTargets);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.index,
        {EHandeSkeletonBone::IndexFinger0, EHandeSkeletonBone::IndexFinger1, EHandeSkeletonBone::IndexFinger2, EHandeSkeletonBone::IndexFinger3, EHandeSkeletonBone::IndexFinger4},
        EHandeSkeletonBone::Aux_IndexFinger,
        saOpenVRBoneTargets);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.middle,
        {EHandeSkeletonBone::MiddleFinger0, EHandeSkeletonBone::MiddleFinger1, EHandeSkeletonBone::MiddleFinger2, EHandeSkeletonBone::MiddleFinger3, EHandeSkeletonBone::MiddleFinger4},
        EHandeSkeletonBone::Aux_MiddleFinger,
        saOpenVRBoneTargets);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.ring,
        {EHandeSkeletonBone::RingFinger0, EHandeSkeletonBone::RingFinger1, EHandeSkeletonBone::RingFinger2, EHandeSkeletonBone::RingFinger3, EHandeSkeletonBone::RingFinger4},
        EHandeSkeletonBone::Aux_RingFinger,
        saOpenVRBoneTargets);

    WriteConvertedDigitToTargetOVRBoneIndicies(
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
    const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
    const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
    const LEAP_DIGIT& sLeapDigit,
    const std::array<EHandeSkeletonBone, 5>& aeDestinationIndicies,
    const EHandeSkeletonBone& eAuxBoneIndex,
    std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>& saOpenVRBoneTargets) inline const {
    ConvertAndWriteBoneToOVRBone(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.metacarpal.prev_joint,
        sLeapDigit.metacarpal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[0])]);

    ConvertAndWriteBoneToOVRBone(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.proximal.prev_joint,
        sLeapDigit.proximal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[1])]);

    ConvertAndWriteBoneToOVRBone(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.intermediate.prev_joint,
        sLeapDigit.intermediate.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[2])]);

    ConvertAndWriteBoneToOVRBone(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.distal.prev_joint,
        sLeapDigit.distal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[3])]);

    ConvertAndWriteBoneToOVRBone(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.distal.next_joint,
        sLeapDigit.distal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[4])]);

    ConvertAndWriteBoneToOVRBone(
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.distal.prev_joint,
        sLeapDigit.distal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(eAuxBoneIndex)]);
}

void northstar::math::CSkeletalAdapter::ConvertAndWriteBoneToOVRBone(
    const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
    const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
    const LEAP_VECTOR& sLeapPosition,
    const LEAP_QUATERNION& sLeapOrientation,
    vr::VRBoneTransform_t& sVRBoneTransform) inline const {
    auto v4dRootPosition = m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace * GetTranslationSensitiveV4DFromLeapVectorPosition(sLeapPosition);
    auto qdRootOrientation = m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace * GetQuaterniondFromLeapQuaternionOrientation(sLeapOrientation);
    WritePositionAndOrientationIntoVRBonePose(
        sVRBoneTransform, v4dRootPosition, Quaterniond(qdRootOrientation.rotation()));
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
        static_cast<float>(v4dPosition.w()) };

    sVRBoneTransform.orientation = { 
        static_cast<float>(qdOrientation.w()), 
        static_cast<float>(qdOrientation.x()), 
        static_cast<float>(qdOrientation.y()), 
        static_cast<float>(qdOrientation.z()) };
}
