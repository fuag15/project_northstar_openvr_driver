#include "math/WorldAdapter.hpp"

using namespace northstar::math::types;

northstar::math::CWorldAdapter::CWorldAdapter(
    std::shared_ptr<IMatrixFactory> pMatrixFactory,
    std::shared_ptr<IVectorFactory> pVectorFactory) {
    m_pMatrixFactory = pMatrixFactory;
    m_pVectorFactory = pVectorFactory;
}

// TODO: Document space (left handed, upside down, x left, y down, z forward)
SPose northstar::math::CWorldAdapter::FromStructureSensorPoseToOpenVRSpace(const ST::XRPose& Pose) {
    static const size_t rows = 4, columns = 4;
    auto m4PoseMatrix = Pose.matrix.m;
    auto adRowMajor = std::array<double, 16>();

    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < columns; c++)
            adRowMajor[(r * rows) + c] = m4PoseMatrix[(c * columns) + r];
 
    auto m4d = m_pMatrixFactory->FromRowMajorArray(adRowMajor);
    auto rotation = Quaterniond(m4d.rotation());
    return {
        Vector3d(-m4d.translation().x(), -m4d.translation().y(), -m4d.translation().z()),
        Quaterniond(rotation.w(), rotation.x(), -rotation.y(), -rotation.z())
    };
}

// TODO: this is jittery
Vector3d northstar::math::CWorldAdapter::FromStructureSensorAngularVectorArrayToOpenVRSpace(const std::array<double, 3>& vdVector) {
    return m_pVectorFactory->V3DFromArray(std::array<double, 3>{-vdVector[0], -vdVector[1], -vdVector[2]});
}

// TODO: this is jittery
Vector3d northstar::math::CWorldAdapter::FromStructureSensorLinearVectorArrayToOpenVRSpace(const std::array<double, 3>& vdVector) {
    return m_pVectorFactory->V3DFromArray(std::array<double, 3>{-vdVector[0], -vdVector[1], -vdVector[2]});
}

/*
The position should be relative to the interpupillary median of the hmd. 
The given matrix can be used before multiplying hmdToWorld from a vr Pose to get OVRWorldCoords.

Leap motions axis relative to the northstar mount are +z down, +x left, +y forward
The units supplied are in millimeters

To convert to hmd relative space you need the following matricies:

mounted sensor space to ovr like space (-z forward, +x right, +y up)
R_OpenVR = rotate -90 about x and 180 about z
R_MountedOrientation = rotate according to the angle offset of the mount (provided by quaternion)
T_Mounted = translation provide relative to hmd interpupillary median.
S_UnitConversion = scale by .001 to go from millimeters to meters

Given a translation matrix:
T_LeapToHmd = S_UnitConversion * T_Mounted * R_MountedOrientation * R_OpenVR

The ovr space coordinates can be given by:
OVR_World = OVR_HmdToWorld * T_LeapToHmd
*/
AffineMatrix4d northstar::math::CWorldAdapter::ConversionMatrixFromLeapMotionTrackingSpaceToHMDRelativeSpace(const Vector3d& v3dHeadRelativeLeapPosition, const Quaterniond& qdHeadRelativeLeapOrientation) {
    static const Quaterniond qdRotateFromLeapMotionSensorSpaceToOpenVRSensorSpace =
        Quaterniond(AngleAxisd(-M_PI, Vector3d::UnitZ())) * Quaterniond(AngleAxisd(-M_PI_2, Vector3d::UnitX()));

    return AffineMatrix4d::Identity()
        * Eigen::Scaling(x_dMillimetersToMeters)
        * Translation3d(v3dHeadRelativeLeapPosition)
        * qdHeadRelativeLeapOrientation
        * qdRotateFromLeapMotionSensorSpaceToOpenVRSensorSpace;
}

/*
Given a pose return a rotation and scaling matrix
*/
AffineMatrix4d northstar::math::CWorldAdapter::ConversionMatrixFromHMDSpaceToOpenVRWorldSpace(const vr::DriverPose_t& sOVRPose) {
    return AffineMatrix4d::Identity()
        * Translation3d(
            m_pVectorFactory->V3DFromArray({ sOVRPose.vecPosition[0], sOVRPose.vecPosition[1], sOVRPose.vecPosition[2] }))
        * Quaterniond(sOVRPose.qRotation.w, sOVRPose.qRotation.x, sOVRPose.qRotation.y, sOVRPose.qRotation.z);
}

/*
Given the supplied conversion matrix from this utility, convert a velocity vector into OVR space
*/
Vector3d northstar::math::CWorldAdapter::FromLeapMotionVelocityToOpenVRVelocity(const AffineMatrix4d& m4dConversionMatrix, const Vector3d& v3dLeapVelocity) {
    auto v4dConverted = Translation3d(m4dConversionMatrix.translation()) * m4dConversionMatrix.rotation() * m_pVectorFactory->V4DFromV3DXYZandW(v3dLeapVelocity, 1);
    return Vector3d(v4dConverted.x(), v4dConverted.y(), v4dConverted.z());
}

/*
Given A Unity Space Quaternion return a OpenVR Quaternion

Unity is left handed +z out, +y up, +x right
OpenVR is right handed +z in, +y up, +x right

therefore negating x and y rotation is needed
*/
Quaterniond northstar::math::CWorldAdapter::FromUnityQuaternionToOpenVRQuaternion(const Quaterniond& qdUnityRotation) {
    return Quaterniond(qdUnityRotation.w(), -qdUnityRotation.x(), -qdUnityRotation.y(), qdUnityRotation.z());
}

/*
Given A Unity Space Position return a OpenVR Position

Unity is left handed +z out, +y up, +x right
OpenVR is right handed +z in, +y up, +x right

therefore inverting z is needed
*/
Vector3d northstar::math::CWorldAdapter::FromUnityPositionToOpenVRPosition(const Vector3d& v3dUnityPosition) {
    return Vector3d(v3dUnityPosition.x(), v3dUnityPosition.y(), -v3dUnityPosition.z());
}

/*
In unity y is not inverted for clip space
*/
Vector4d northstar::math::CWorldAdapter::FromUnityProjectionExtentsLRTBToOpenVRProjectionExtentsLRTB(const Vector4d& v4dUnityProjectionExtentsLRTB) {
    return Vector4d(v4dUnityProjectionExtentsLRTB.x(), v4dUnityProjectionExtentsLRTB.y(), v4dUnityProjectionExtentsLRTB.w(), v4dUnityProjectionExtentsLRTB.z());
}

/*
Unity is left handed with z forward, Openvr is right handed with z back

Therefore negate z position and invert x / y rotation
*/
AffineMatrix4d northstar::math::CWorldAdapter::FromUnityMatrix4dToOpenVRMatrix4d(const AffineMatrix4d& v4dUnityPose) {
    auto v3dTranslation = v4dUnityPose.translation().array();
    auto qdRotation = Quaterniond(v4dUnityPose.rotation());
    return Translation3d(v3dTranslation.x(), v3dTranslation.y(), -v3dTranslation.z()) 
        * Quaterniond(qdRotation.w(), -qdRotation.x(), -qdRotation.y(), qdRotation.z())
        * AffineMatrix4d::Identity();
}

/*
Unity UV origin is lower Left, OpenVR uses an upper left origin
*/
Vector2d northstar::math::CWorldAdapter::FromOpenGLUVToUnityUV(const Vector2d& v2dOpenGLUV) {
    return Vector2d(v2dOpenGLUV.x(), 1.0 - v2dOpenGLUV.y());
}

/*
Unity UV origin is lower Left, OpenVR uses an upper left origin
*/
Vector2d northstar::math::CWorldAdapter::FromUnityUVToOpenGLUV(const Vector2d& v2dUnityUV) {
    return Vector2d(v2dUnityUV.x(), 1.0 - v2dUnityUV.y());
}
