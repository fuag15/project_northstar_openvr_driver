#include "math/SkeletalAdapter.hpp"

using namespace northstar::driver::types;
using namespace northstar::math::types;
using namespace northstar::utility;

northstar::math::CSkeletalAdapter::CSkeletalAdapter(
    std::shared_ptr<IVectorFactory> pVectorFactory, 
    std::shared_ptr<IMatrixFactory> pMatrixFactory,
    std::shared_ptr<ILogger> pLogger) {
    m_pVectorFactory = pVectorFactory;
    m_pMatrixFactory = pMatrixFactory;
    m_pLogger = pLogger;
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
    std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>& saOpenVRBoneTargets,
    const EHand& eHand) const {
    AffineMatrix4d m4dWorldToHandRelativeSpace = m_pMatrixFactory->FromTraslationAndRotation(
        m_pVectorFactory->V3DFromArray({ sPose.vecPosition[0], sPose.vecPosition[1], sPose.vecPosition[2] }),
        Quaterniond(sPose.qRotation.w, sPose.qRotation.x, sPose.qRotation.y, sPose.qRotation.z)).inverse();

    // Root is always at /pose/raw anchor point
    static const auto v4dRootPosition = m_pVectorFactory->V4DFromXYZWArray({ 0, 0, 0, 1 });
    static const auto qdRootOrientation = Quaterniond::Identity();
    WritePositionAndOrientationIntoVRBonePose(
        saOpenVRBoneTargets[static_cast<int32_t>(EHandeSkeletonBone::Root)],
        v4dRootPosition,
        qdRootOrientation);

    AffineMatrix4d m4dWorldToRootBoneRelativeSpace = m4dWorldToHandRelativeSpace;

    // Wrist, invert the orientation from leap as leap sees this as oriented away from the palm and ovr sees this as oriented away from the elbow
    auto v4dWristWorldPosition = m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace * GetTranslationSensitiveV4DFromLeapVectorPosition(sLeapHand.arm.next_joint);
    auto qdWristWorldOrientation = m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace * GetQuaterniondFromLeapQuaternionOrientation(sLeapHand.palm.orientation);
    
    // TODO: CLean up
    if (eHand == EHand::Right) {
        auto wristYAxis = qdWristWorldOrientation.rotation() * Vector3d::UnitY();
        auto rotation = Quaterniond(AngleAxisd(M_PI, wristYAxis));
        auto wristZAxis = qdWristWorldOrientation.rotation() * Vector3d::UnitZ();
        auto rotation2 = Quaterniond(AngleAxisd(M_PI_2, wristZAxis));
        qdWristWorldOrientation = rotation2 * rotation * qdWristWorldOrientation;
    } else { // EHand::Left
        auto wristYAxis = qdWristWorldOrientation.rotation() * Vector3d::UnitY();
        auto rotation = Quaterniond(AngleAxisd(M_PI, wristYAxis));
        auto wristZAxis = qdWristWorldOrientation.rotation() * Vector3d::UnitZ();
        auto rotation2 = Quaterniond(AngleAxisd(-M_PI_2, wristZAxis));
        qdWristWorldOrientation = rotation2 * rotation * qdWristWorldOrientation;
    }

    auto v4dRootRelativeWristPosition = m4dWorldToRootBoneRelativeSpace * v4dWristWorldPosition;
    auto qdRootRelativeWristOrientation = m4dWorldToRootBoneRelativeSpace * qdWristWorldOrientation;
    WritePositionAndOrientationIntoVRBonePose(
        saOpenVRBoneTargets[static_cast<int32_t>(EHandeSkeletonBone::Wrist)],
        v4dRootRelativeWristPosition,
        Quaterniond(qdRootRelativeWristOrientation.rotation()));

    // Get world to wrist relative space now
    AffineMatrix4d m4dWorldToWristBoneRelativeSpace = m_pMatrixFactory->FromTraslationAndRotation(
        m_pVectorFactory->V3DXYZFromV4D(v4dWristWorldPosition),
        Quaterniond(qdWristWorldOrientation.rotation())).inverse();

    WriteConvertedDigitToTargetOVRBoneIndicies(
        m4dWorldToRootBoneRelativeSpace,
        m4dWorldToWristBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.thumb,
        {EHandeSkeletonBone::Wrist, EHandeSkeletonBone::Thumb0, EHandeSkeletonBone::Thumb1, EHandeSkeletonBone::Thumb2, EHandeSkeletonBone::Thumb3},
        EHandeSkeletonBone::Aux_Thumb,
        saOpenVRBoneTargets,
        EBoneType::Thumb,
        eHand);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        m4dWorldToRootBoneRelativeSpace,
        m4dWorldToWristBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.index,
        {EHandeSkeletonBone::IndexFinger0, EHandeSkeletonBone::IndexFinger1, EHandeSkeletonBone::IndexFinger2, EHandeSkeletonBone::IndexFinger3, EHandeSkeletonBone::IndexFinger4},
        EHandeSkeletonBone::Aux_IndexFinger,
        saOpenVRBoneTargets,
        EBoneType::Digit,
        eHand);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        m4dWorldToRootBoneRelativeSpace,
        m4dWorldToWristBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.middle,
        {EHandeSkeletonBone::MiddleFinger0, EHandeSkeletonBone::MiddleFinger1, EHandeSkeletonBone::MiddleFinger2, EHandeSkeletonBone::MiddleFinger3, EHandeSkeletonBone::MiddleFinger4},
        EHandeSkeletonBone::Aux_MiddleFinger,
        saOpenVRBoneTargets,
        EBoneType::Digit,
        eHand);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        m4dWorldToRootBoneRelativeSpace,
        m4dWorldToWristBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.ring,
        {EHandeSkeletonBone::RingFinger0, EHandeSkeletonBone::RingFinger1, EHandeSkeletonBone::RingFinger2, EHandeSkeletonBone::RingFinger3, EHandeSkeletonBone::RingFinger4},
        EHandeSkeletonBone::Aux_RingFinger,
        saOpenVRBoneTargets,
        EBoneType::Digit,
        eHand);

    WriteConvertedDigitToTargetOVRBoneIndicies(
        m4dWorldToRootBoneRelativeSpace,
        m4dWorldToWristBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapHand.pinky,
        {EHandeSkeletonBone::PinkyFinger0, EHandeSkeletonBone::PinkyFinger1, EHandeSkeletonBone::PinkyFinger2, EHandeSkeletonBone::PinkyFinger3, EHandeSkeletonBone::PinkyFinger4},
        EHandeSkeletonBone::Aux_PinkyFinger,
        saOpenVRBoneTargets,
        EBoneType::Digit,
        eHand);
}

// TODO: Clean this up
/*
it is assumed that the index order is in the order of leap data

Metacarpal, Poximal, Intermediate, distal phalanges.

with the fifth being the end of the Digit
*/
void northstar::math::CSkeletalAdapter::WriteConvertedDigitToTargetOVRBoneIndicies(
    const types::AffineMatrix4d& m4dFromWorldToRootBoneRelativeSpace,
    const types::AffineMatrix4d& m4dFromWorldToWristBoneRelativeSpace,
    const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
    const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
    const LEAP_DIGIT& sLeapDigit,
    const std::array<EHandeSkeletonBone, 5>& aeDestinationIndicies,
    const EHandeSkeletonBone& eAuxBoneIndex,
    std::array<vr::VRBoneTransform_t, northstar::driver::settings::values::driverConfiguration::k_unBoneCount>& saOpenVRBoneTargets,
    const EBoneType& eBoneType,
    const EHand& eHand) inline const {
    auto m4dCurrentWorldToParentRelativeSpace = m4dFromWorldToWristBoneRelativeSpace;
    m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dCurrentWorldToParentRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.metacarpal.prev_joint,
        sLeapDigit.metacarpal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[0])],
        eBoneType,
        ESegmentClassification::Metacarpal,
        eHand);

    m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dCurrentWorldToParentRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.proximal.prev_joint,
        sLeapDigit.proximal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[1])],
        eBoneType,
        ESegmentClassification::Proximal,
        eHand);

    m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dCurrentWorldToParentRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.intermediate.prev_joint,
        sLeapDigit.intermediate.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[2])],
        eBoneType,
        ESegmentClassification::Intermediate,
        eHand);

    m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dCurrentWorldToParentRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.distal.prev_joint,
        sLeapDigit.distal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[3])],
        eBoneType,
        ESegmentClassification::Distal,
        eHand);

    m4dCurrentWorldToParentRelativeSpace = ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dCurrentWorldToParentRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.distal.next_joint,
        sLeapDigit.distal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(aeDestinationIndicies[4])],
        eBoneType,
        ESegmentClassification::Tip,
        eHand);

    ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
        m4dFromWorldToRootBoneRelativeSpace,
        m4dFromLeapSensorToHMDRelativeSpace,
        m4dFromHMDToWorldSpace,
        sLeapDigit.distal.prev_joint,
        sLeapDigit.distal.rotation,
        saOpenVRBoneTargets[static_cast<int32_t>(eAuxBoneIndex)],
        eBoneType,
        ESegmentClassification::Auxilary,
        eHand);
}

AffineMatrix4d northstar::math::CSkeletalAdapter::ConvertAndWriteBoneToOVRBoneReturningNewWorldToRelativeTransform(
    const types::AffineMatrix4d& m4dFromWorldSpaceToParentRelativeSpace,
    const types::AffineMatrix4d& m4dFromLeapSensorToHMDRelativeSpace,
    const types::AffineMatrix4d& m4dFromHMDToWorldSpace,
    const LEAP_VECTOR& sLeapPosition,
    const LEAP_QUATERNION& sLeapOrientation,
    vr::VRBoneTransform_t& sVRBoneTransform,
    const EBoneType& eBoneType,
    const ESegmentClassification& eSegmentClassification,
    const EHand& eHand) inline const {
    // thumbs ignore metacarpal
    if (eBoneType == EBoneType::Thumb 
        && eSegmentClassification == ESegmentClassification::Metacarpal)
        return m4dFromWorldSpaceToParentRelativeSpace;

    const auto m4dFromLeapSensorToWorldSpace = m4dFromHMDToWorldSpace * m4dFromLeapSensorToHMDRelativeSpace;
    auto v4dWorldSpaceRootPosition = m4dFromLeapSensorToWorldSpace * GetTranslationSensitiveV4DFromLeapVectorPosition(sLeapPosition);
    auto qdWorldSpaceRootOrientation = m4dFromLeapSensorToWorldSpace * GetQuaterniondFromLeapQuaternionOrientation(sLeapOrientation);
    auto qdBoneSpaceRootOrientation = ConvertLeapDigitOrientationToOpenVRDigitOrientation(
        Quaterniond(qdWorldSpaceRootOrientation.rotation()),
        eBoneType,
        eSegmentClassification,
        eHand);

    auto qdParentSpaceRootOrientation = m4dFromWorldSpaceToParentRelativeSpace * qdBoneSpaceRootOrientation;
    auto v4dParentSpaceRootPosition = m4dFromWorldSpaceToParentRelativeSpace * v4dWorldSpaceRootPosition;
    WritePositionAndOrientationIntoVRBonePose(
        sVRBoneTransform, 
        v4dParentSpaceRootPosition, 
        Quaterniond(qdParentSpaceRootOrientation.rotation()));

    return m_pMatrixFactory->FromTraslationAndRotation(
        m_pVectorFactory->V3DXYZFromV4D(v4dWorldSpaceRootPosition),
        qdBoneSpaceRootOrientation).inverse();
}

// TODO: Clean up
Quaterniond northstar::math::CSkeletalAdapter::ConvertLeapDigitOrientationToOpenVRDigitOrientation(
    const Quaterniond& qdLeapDigitOrientation, 
    const EBoneType& eBoneType,
    const ESegmentClassification& eSegmentClassification,
    const EHand& eHand) const {
    auto qdOpenVRSpaceOrientation = qdLeapDigitOrientation;
    if (eHand == EHand::Right) {
        auto v3dLocalYAxis = qdOpenVRSpaceOrientation * Vector3d::UnitY();
        auto qdLocalRotationY = Quaterniond(AngleAxisd(-M_PI_2, v3dLocalYAxis));
        qdOpenVRSpaceOrientation = qdLocalRotationY * qdOpenVRSpaceOrientation;
    } else {// EHand::Left
        auto v3dLocalYAxis = qdOpenVRSpaceOrientation * Vector3d::UnitY();
        auto qdLocalRotationY = Quaterniond(AngleAxisd(M_PI_2, v3dLocalYAxis));
        auto v3dLocalZAxis = qdOpenVRSpaceOrientation * Vector3d::UnitZ();
        auto qdLocalRotationZ = Quaterniond(AngleAxisd(M_PI, v3dLocalZAxis));
        qdOpenVRSpaceOrientation = qdLocalRotationZ * qdLocalRotationY * qdOpenVRSpaceOrientation;
    }

    return qdOpenVRSpaceOrientation;
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
