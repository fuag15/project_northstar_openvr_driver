#include "driver/2DCalibratedOptics.hpp"

using namespace northstar::math::types;
using namespace northstar::driver::settings::keys;

// TODO: Make this 2 instances and choose on instantiation
northstar::driver::C2DCalibratedOptics::C2DCalibratedOptics(
    vr::IVRSettings* pVRSettings,
    std::shared_ptr<northstar::math::IWorldAdapter> pWorldAdapter,
    std::shared_ptr<northstar::math::ISpaceAdapter> pSpaceAdapter,
    std::shared_ptr<northstar::math::IGeometry> pGeometry,
    std::shared_ptr<northstar::math::IMatrixFactory> pMatrixFactory,
    std::shared_ptr<northstar::math::IVectorFactory> pVectorFactory,
    std::shared_ptr<northstar::utility::ILogger> pLogger) {
    m_pVRSettings = pVRSettings;
    m_pWorldAdapter = pWorldAdapter;
    m_pSpaceAdapter = pSpaceAdapter;
    m_pGeometry = pGeometry;
    m_pVectorFactory = pVectorFactory;
    m_pMatrixFactory = pMatrixFactory;
    m_pLogger = pLogger;

    for (const auto& eEye : { vr::Eye_Left, vr::Eye_Right }) {
        m_umEyeConfigs.insert({ eEye, LoadConfigFromEye(eEye) });
        m_umUVWarps.insert({ eEye, {} });
    }
}

northstar::driver::C2DCalibratedOptics::SEyeConfiguration northstar::driver::C2DCalibratedOptics::LoadConfigFromEye(const vr::EVREye& eEye) {
    auto sEyeKey = eEye == vr::Eye_Left ? eye2D::k_svRootLeft : eye2D::k_svRootRight;
    SEyeConfiguration sEyeConfiguration;
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[0] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_0.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[1] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_1.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[2] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_2.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[3] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_3.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[4] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_4.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[5] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_5.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[6] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_6.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[7] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_7.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[8] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_8.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[9] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_9.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[10] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_10.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[11] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_11.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[12] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_12.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[13] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_13.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[14] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_14.data());
    sEyeConfiguration.UVToRectilinearXAxisCoefficients[15] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_15.data());

    sEyeConfiguration.UVToRectilinearYAxisCoefficients[0] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_0.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[1] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_1.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[2] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_2.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[3] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_3.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[4] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_4.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[5] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_5.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[6] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_6.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[7] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_7.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[8] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_8.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[9] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_9.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[10] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_10.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[11] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_11.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[12] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_12.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[13] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_13.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[14] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_14.data());
    sEyeConfiguration.UVToRectilinearYAxisCoefficients[15] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_15.data());

    sEyeConfiguration.m4dCameraProjection =
        m_pMatrixFactory->FromRowMajorArray(
            { m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_00.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_01.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_02.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_03.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_10.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_11.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_12.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_13.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_20.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_21.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_22.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_23.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_30.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_31.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_32.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjection_33.data()) });

    // NOTE the monado driver keeps all these a 0.6 e.g. {-0.6, 0.6, 0.6, -0.6}
    sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB =
        m_pWorldAdapter->FromUnityProjectionExtentsLRTBToOpenVRProjectionExtentsLRTB(
            m_pVectorFactory->V4DFromXYZWArray(
                { m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjectionX.data())
                , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjectionY.data())
                , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjectionZ.data())
                , m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svCameraProjectionW.data()) }));

    return sEyeConfiguration;
}

// TODO: What are these in v2 calibration
Vector4d northstar::driver::C2DCalibratedOptics::GetEyeProjectionLRTB(const vr::EVREye& eEye) {
    const auto& sEyeConfiguration = m_umEyeConfigs.at(eEye);
    return sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB;
}

// TODO: WIP
Vector2d northstar::driver::C2DCalibratedOptics::EyeUVToScreenUV(const vr::EVREye& eEye, const Vector2d& v2dTargetEyeUV) {
    auto& pWarps = m_umUVWarps.at(eEye);
    auto pWarp = pWarps.find(v2dTargetEyeUV);
    if (pWarp != pWarps.end()) {
        return pWarp->second;
    }

    auto v2dSolvedWarpUV = PolynomialWarpSolve(eEye, v2dTargetEyeUV);
    pWarps.insert_or_assign(v2dTargetEyeUV, v2dSolvedWarpUV);
    return v2dSolvedWarpUV;
}

// TODO: Understand
// TODO: Credit Noah / Bryan for this
/*
# NOTES FROM DIGGING
1. In steamVR UV origin is the upper left, In the shader toy example UV origin is in the lower right
=> u and v must be negated before sampling the polynomial

2. in the shadertoy example, the left side samples the right eye and the right side samples the left eye
=> left eye should use right eye coeff, and right eye should use left eye coeff

3. in the shadertoy example, when a direction is sampled, y is used to indicate looking left and right, x is used to indicate looking up and down
=> UDirection and VDirection are swapped

4. the rays are written as though x is up and y is left, but steamvr uvs are opposite
=> the ray directions must be negated

5. it appears the constants in the example shader toy were made for a monitor that was flipped 180 where as the ones i got were made for a monitor that was not flipped
=> ???

6. it seems given testing of the shadertoy values that the rays are bounded to [-1, 1] on each axis
=> things are shifted from [-1, 1] -> [0, 1]
*/
// TODO: double check that the frustum extents look right on the headset when end to end testing is possible, ask for answers to these questions
Vector2d northstar::driver::C2DCalibratedOptics::PolynomialWarpSolve(const vr::EVREye& eEye, const Vector2d& v2dTargetEyeUV) const noexcept {
    const auto& sEyeConfiguration = eEye == vr::EVREye::Eye_Left ? m_umEyeConfigs.at(vr::EVREye::Eye_Right) : m_umEyeConfigs.at(vr::EVREye::Eye_Left);
    //const auto& sEyeConfiguration = m_umEyeConfigs.at(eEye);
    // TODO: One solution claims this is the ray dir from eye origin
    // the other uses this as the angle of offsset in radians from the forward direction in the projection
    //return Vector2d{ v2dTargetEyeUV.x() + 0.5, v2dTargetEyeUV.y() + 0.5 };
    const float fRayDirectionAlongUAxis = -EvaluateThirdDegree2DPolynomial(1.0 - v2dTargetEyeUV.x(), 1.0 - v2dTargetEyeUV.y(), sEyeConfiguration.UVToRectilinearXAxisCoefficients);
    const float fRayDirectionAlongVAxis = -EvaluateThirdDegree2DPolynomial(1.0 - v2dTargetEyeUV.x(), 1.0 - v2dTargetEyeUV.y(), sEyeConfiguration.UVToRectilinearYAxisCoefficients);

    //float fViewFrustumLeftAngleExtentInRadians = tan(sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB.x());
    //float fViewFrustumRightAngleExtentInRadians = tan(sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB.y());
    //float fViewFrustumUpAngleExtentInRadians = tan(sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB.z());
    //float fViewFrustumDownAngleExtentInRadians = tan(sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB.w());

    // Treat ray axis along u direction as though it were a linear interpolation from 0 - 1 along the combined left / right frustrum radian extents?
    // This seems odd unsure what to call this variable, this is how monado driver does things
    //float fFinalU = (fRayDirectionAlongUAxis + fViewFrustumRightAngleExtentInRadians) / (fViewFrustumRightAngleExtentInRadians - fViewFrustumLeftAngleExtentInRadians);

    // Treat ray axis along v direction as though it were a linear interpolation from 0 - 1 along the combined top / bottom frustrum radian extents?
    // This seems odd unsure what to call this variable, this is how monado driver does things
    //float fFinalV = (fRayDirectionAlongVAxis + fViewFrustumUpAngleExtentInRadians) / (fViewFrustumUpAngleExtentInRadians - fViewFrustumDownAngleExtentInRadians);
    //return Vector2d{ fFinalV, fFinalU };

    // shift from [-1, 1] 
    return Vector2d{ ((fRayDirectionAlongVAxis / 2.0) + 0.5), ((fRayDirectionAlongUAxis / 2.0) + 0.5) };
}

// TODO: better name for this and put it in geometry?
// TODO: Credit Noah / Bryan for this
float northstar::driver::C2DCalibratedOptics::EvaluateThirdDegree2DPolynomial(float XPosition, float YPosition, const std::array<float, 16>& Coefficients) const noexcept
{
    const float XPositionSquared = XPosition * XPosition;
    const float XPositionCubed = XPosition * XPosition * XPosition;
    const float YPositionSquared = YPosition * YPosition;
    const float YPositionCubed = YPosition * YPosition * YPosition;
    return (
        (Coefficients[0] + (Coefficients[1] * YPosition) + (Coefficients[2] * YPositionSquared) + (Coefficients[3] * YPositionCubed)) +
        ((Coefficients[4] * XPosition) + (Coefficients[5] * XPosition * YPosition) + (Coefficients[6] * XPosition * YPositionSquared) + (Coefficients[7] * XPosition * YPositionCubed)) +
        ((Coefficients[8] * XPositionSquared) + (Coefficients[9] * XPositionSquared * YPosition) + (Coefficients[10] * XPositionSquared * YPositionSquared) + (Coefficients[11] * XPositionSquared * YPositionCubed)) +
        ((Coefficients[12] * XPositionCubed) + (Coefficients[13] * XPositionCubed * YPosition) + (Coefficients[14] * XPositionCubed * YPositionSquared) + (Coefficients[15] * XPositionCubed * YPositionCubed))
    );
}
