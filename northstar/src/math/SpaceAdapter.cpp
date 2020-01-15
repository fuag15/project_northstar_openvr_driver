#include "math/SpaceAdapter.hpp"

using northstar::math::types::Vector3d;
using northstar::math::types::Vector2d;
using northstar::math::types::ProjMatrix4d;

northstar::math::CSpaceAdapter::CSpaceAdapter(std::shared_ptr<IVectorFactory> pVectorFactory)
{
    m_pVectorFactory = pVectorFactory;
}

Vector3d northstar::math::CSpaceAdapter::FromUnityUVToUnityClipUnitForwardZ(const Vector2d& v2dUnityUV) {
    const auto v2dShiftRange = m_pVectorFactory->V2DFromArray({ 0.5, 0.5 });
    auto v2dShifted = (v2dUnityUV - v2dShiftRange) * 2.0;
    return m_pVectorFactory->V3DFromArray({ v2dShifted.x(), v2dShifted.y(), 1.0 });
}

Vector2d northstar::math::CSpaceAdapter::FromUnityClipNearPlaneToUnityUV(const Vector3d& v3dUnityClip) {
    const auto v2dShiftRange = m_pVectorFactory->V2DFromArray({ 0.5, 0.5 });
    return Vector2d(v3dUnityClip.x(), v3dUnityClip.y()) + v2dShiftRange;
}

Vector3d northstar::math::CSpaceAdapter::ProjectPoint(const ProjMatrix4d& m4dProjection, const Vector3d& v3dPoint) {
    auto v4dProjected = m4dProjection * m_pVectorFactory->V4DFromV3DXYZandW(v3dPoint, 1.0);
    return v4dProjected.block<3, 1>(0, 0).array() / v4dProjected.w();
}
