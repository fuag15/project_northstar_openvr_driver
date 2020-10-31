#include "driver/Optics.hpp"

using namespace northstar::math::types;
using namespace northstar::driver::settings::keys;

// TODO: Make this 2 instances and choose on instantiation
northstar::driver::COptics::COptics(
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
    m_eTargetConfigurationData = DetermineTargetConfigurationType();

    //if (m_eTargetConfigurationData == ETargetConfigurationData::ThreeDee) {
    for (const auto& eEye : { vr::Eye_Left, vr::Eye_Right }) {
        m_umEyeConfigs3D.insert({ eEye, Load3DConfigFromEye(eEye) });
        m_umUVWarps.insert({ eEye, {} });
    }
    //}
    //else {
    for (const auto& eEye : { vr::Eye_Left, vr::Eye_Right }) {
        m_umEyeConfigs2D.insert({ eEye, Load2DConfigFromEye(eEye) });
        m_umUVWarps.insert({ eEye, {} });
    }
    //}
}

northstar::driver::COptics::ETargetConfigurationData northstar::driver::COptics::DetermineTargetConfigurationType() {
    const bool use2DConfigurationData = m_pVRSettings->GetBool(configuration::k_svRoot.data(), configuration::k_svUse2DCalibrationData.data());
    return use2DConfigurationData ? ETargetConfigurationData::TwoDee : ETargetConfigurationData::ThreeDee;
}

northstar::driver::COptics::SEyeConfiguration2D northstar::driver::COptics::Load2DConfigFromEye(const vr::EVREye& eEye) {
    auto sEyeKey = eEye == vr::Eye_Left ? eye2D::k_svRootLeft : eye2D::k_svRootRight;
    SEyeConfiguration2D sEyeConfiguration2D;
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[0] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_0.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[1] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_1.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[2] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_2.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[3] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_3.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[4] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_4.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[5] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_5.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[6] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_6.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[7] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_7.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[8] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_8.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[9] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_9.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[10] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_10.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[11] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_11.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[12] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_12.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[13] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_13.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[14] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_14.data());
    sEyeConfiguration2D.UVToRectilinearXAxisCoefficients[15] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearXAxisCoefficient_15.data());

    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[0] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_0.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[1] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_1.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[2] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_2.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[3] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_3.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[4] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_4.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[5] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_5.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[6] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_6.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[7] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_7.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[8] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_8.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[9] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_9.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[10] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_10.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[11] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_11.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[12] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_12.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[13] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_13.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[14] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_14.data());
    sEyeConfiguration2D.UVToRectilinearYAxisCoefficients[15] = m_pVRSettings->GetFloat(sEyeKey.data(), eye2D::k_svUVToRectilinearYAxisCoefficient_15.data());

    sEyeConfiguration2D.m4dCameraProjection =
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

    return sEyeConfiguration2D;
}

northstar::driver::COptics::SEyeConfiguration3D northstar::driver::COptics::Load3DConfigFromEye(const vr::EVREye& eEye) {
    auto sEyeKey = eEye == vr::Eye_Left ? eye3D::k_svRootLeft : eye3D::k_svRootRight;
    SEyeConfiguration3D sEyeConfiguration3D;
    sEyeConfiguration3D.sEllipsisAxis.dMajor = m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEllipseMajorAxis.data());
    sEyeConfiguration3D.sEllipsisAxis.dMinor = m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEllipseMinorAxis.data());
    sEyeConfiguration3D.v3dScreenForward =
        m_pVectorFactory->V3DFromArray(
            { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenForwardX.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenForwardY.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenForwardZ.data()) });

    sEyeConfiguration3D.v3dScreenPosition =
        m_pVectorFactory->V3DFromArray(
            { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenPositionX.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenPositionY.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenPositionZ.data()) });

    sEyeConfiguration3D.v3dEyePosition =
        m_pVectorFactory->V3DFromArray(
            { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyePositionX.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyePositionY.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyePositionZ.data()) });

    sEyeConfiguration3D.qdEyeRotation =
        Quaterniond(
            m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyeRotationW.data()),
            m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyeRotationX.data()),
            m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyeRotationY.data()),
            m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyeRotationZ.data()));

    sEyeConfiguration3D.v4dCameraProjectionFrustumExtentsLRTB =
        m_pWorldAdapter->FromUnityProjectionExtentsLRTBToOpenVRProjectionExtentsLRTB(
            m_pVectorFactory->V4DFromXYZWArray(
                { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svCameraProjectionX.data())
                , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svCameraProjectionY.data())
                , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svCameraProjectionZ.data())
                , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svCameraProjectionW.data()) }));

    sEyeConfiguration3D.m4dSphereToWorldSpace =
        m_pMatrixFactory->FromRowMajorArray(
            { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_00.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_01.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_02.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_03.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_10.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_11.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_12.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_13.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_20.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_21.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_22.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svSphereToWorldSpace_23.data())
            , 0, 0, 0, 1 });

    sEyeConfiguration3D.m4dWorldToScreenSpace =
        m_pMatrixFactory->FromRowMajorArray(
            { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_00.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_01.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_02.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_03.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_10.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_11.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_12.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_13.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_20.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_21.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_22.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svWorldToScreenSpace_23.data())
            , 0, 0, 0, 1 });

    sEyeConfiguration3D.m4dWorldToSphereSpace = sEyeConfiguration3D.m4dSphereToWorldSpace.inverse();
    sEyeConfiguration3D.m4dCameraProjection = 
        m_pMatrixFactory->OpenGLProjectionFromFrustumExtents(
            sEyeConfiguration3D.v4dCameraProjectionFrustumExtentsLRTB.x(),
            sEyeConfiguration3D.v4dCameraProjectionFrustumExtentsLRTB.y(),
            sEyeConfiguration3D.v4dCameraProjectionFrustumExtentsLRTB.z(),
            sEyeConfiguration3D.v4dCameraProjectionFrustumExtentsLRTB.w());

    auto m4dEyeTranslation = m_pMatrixFactory->TranslationFromVector3d(sEyeConfiguration3D.v3dEyePosition);
    auto m4dEyeToWorld = m4dEyeTranslation * sEyeConfiguration3D.qdEyeRotation;
    m4dEyeToWorld.matrix().col(2) *= -1.0;
    sEyeConfiguration3D.m4dClipToWorld = m4dEyeToWorld * sEyeConfiguration3D.m4dCameraProjection.inverse();
    return sEyeConfiguration3D;
}

Vector4d northstar::driver::COptics::GetEyeProjectionLRTB(const vr::EVREye& eEye) {
    // TODO: what is this for 2D calibraiton?
    if (m_eTargetConfigurationData == ETargetConfigurationData::ThreeDee) {
        return GetEyeProjectionLRTBVia3DConfiguration(eEye);
    }
    else {
        return GetEyeProjectionLRTBVia2DConfiguration(eEye);
    }

    return m_umEyeConfigs3D.at(eEye).v4dCameraProjectionFrustumExtentsLRTB;
}

Vector4d northstar::driver::COptics::GetEyeProjectionLRTBVia3DConfiguration(const vr::EVREye& eEye) {
    return m_umEyeConfigs3D.at(eEye).v4dCameraProjectionFrustumExtentsLRTB;
}

Vector4d northstar::driver::COptics::GetEyeProjectionLRTBVia2DConfiguration(const vr::EVREye& eEye) {
    // TODO: Determine these?
    return m_umEyeConfigs3D.at(eEye).v4dCameraProjectionFrustumExtentsLRTB;
}

Vector2d northstar::driver::COptics::EyeUVToScreenUV(const vr::EVREye& eEye, const Vector2d& v2dTargetEyeUV) {
    if (m_eTargetConfigurationData == ETargetConfigurationData::ThreeDee) {
        return EyeUVToScreenUVVia3DConfiguration(eEye, v2dTargetEyeUV);
    }
    else {
        return EyeUVToScreenUVVia2DConfiguration(eEye, v2dTargetEyeUV);
    }
}

Vector2d northstar::driver::COptics::EyeUVToScreenUVVia2DConfiguration(const vr::EVREye& eEye, const Vector2d& v2dTargetEyeUV) {
    return Vector2d{ 0.0, 0.0 };
}

// NOTE the solve happens in Unity space
Vector2d northstar::driver::COptics::EyeUVToScreenUVVia3DConfiguration(const vr::EVREye& eEye, const Vector2d& v2dTargetEyeUV) {
    static const auto s_v2dInitialWarpGuess = m_pVectorFactory->V2DFromArray(x_daInitialWarpGuess);
    auto& pWarps = m_umUVWarps.at(eEye);
    auto v2dUnitySpaceTargetEyeUV = m_pWorldAdapter->FromOpenGLUVToUnityUV(v2dTargetEyeUV);
    auto pWarp = pWarps.find(v2dUnitySpaceTargetEyeUV);
    auto v2dCurrentWarpGuess = pWarp == pWarps.end() ? s_v2dInitialWarpGuess : pWarp->second;
    auto v2dSolvedWarpGuess = IterativeGradientUVWarpSolve(eEye, v2dUnitySpaceTargetEyeUV, v2dCurrentWarpGuess);
    pWarps.insert_or_assign(v2dUnitySpaceTargetEyeUV, v2dSolvedWarpGuess);
    return m_pWorldAdapter->FromUnityUVToOpenGLUV(v2dSolvedWarpGuess);
}

// NOTE: the solve happens in Unity space
// CREDIT: This was made w.r.t. the reference optics solve in https://github.com/leapmotion/ProjectNorthStar/
Vector2d northstar::driver::COptics::IterativeGradientUVWarpSolve(
    const vr::EVREye& eEye, 
    const Vector2d& v2dTargetEyeUV, 
    const Vector2d& v2dWarpGuess) {
    static const auto s_v2dGradientEpsilonU = m_pVectorFactory->V2DFromArray({ x_dIterativeSolverGradiantEpsilon, 0 }),
        s_v2dGradientEpsilonV = m_pVectorFactory->V2DFromArray({ 0, x_dIterativeSolverGradiantEpsilon });

    auto v2dEyeUV = v2dWarpGuess;
    for (size_t i = 0; i < x_nNumberOfSolverIterations; i++) {
        auto v2dSourceDisplayUV = ReverseProjectEyeUVToDisplayUV(eEye, v2dEyeUV);
        auto v2dLocalSourceDisplayReflectionGradientU = (
            ReverseProjectEyeUVToDisplayUV(
                eEye, 
                v2dEyeUV + s_v2dGradientEpsilonU
            ) - v2dSourceDisplayUV) / x_dIterativeSolverGradiantEpsilon;

        auto v2dLocalSourceDisplayReflectionGradientV = (
            ReverseProjectEyeUVToDisplayUV(
                eEye, 
                v2dEyeUV + s_v2dGradientEpsilonV
            ) - v2dSourceDisplayUV) / x_dIterativeSolverGradiantEpsilon;

        auto v2dError = v2dSourceDisplayUV - v2dTargetEyeUV;
        auto v2dStep = m_pVectorFactory->V2DFromArray({ 0, 0 });
        v2dStep += v2dLocalSourceDisplayReflectionGradientU * v2dError.x();
        v2dStep += v2dLocalSourceDisplayReflectionGradientV * v2dError.y();
        v2dEyeUV -= v2dStep * x_dIterativeSolverStepWeight;
    }
    
    return v2dEyeUV;
}

// NOTE: the solve happens in Unity space
// CREDIT: This was made w.r.t. the reference optics solve in https://github.com/leapmotion/ProjectNorthStar/
Vector2d northstar::driver::COptics::ReverseProjectEyeUVToDisplayUV(const vr::EVREye& eEye, const Vector2d& v2dEyeUV) {
    static const auto s_v2dErrorResult = m_pVectorFactory->V2DFromArray(x_daIterativeSolverErrorResult);
    static const SSphere s_sLensProxySphere = {
        m_pVectorFactory->V3DFromArray(x_daLensProxySphereOrigin),
        x_dLensProxySphereRadius };

    auto sEyeConfiguration = m_umEyeConfigs3D.at(eEye);
    static const auto s_v3dEllipsoidSphereWarp = m_pVectorFactory->V3DFromArray({
        pow(sEyeConfiguration.sEllipsisAxis.dMinor / 2.0, 2.0),
        pow(sEyeConfiguration.sEllipsisAxis.dMinor / 2.0, 2.0),
        pow(sEyeConfiguration.sEllipsisAxis.dMajor / 2.0, 2.0) });

    auto v3dClipSpace = m_pSpaceAdapter->FromUnityUVToUnityClipUnitForwardZ(v2dEyeUV);
    auto v3dRayDirection = (
        m_pSpaceAdapter->ProjectPoint(sEyeConfiguration.m4dClipToWorld, v3dClipSpace)
        - sEyeConfiguration.v3dEyePosition).normalized();

    auto v3dSphereSpaceRayOrigin = sEyeConfiguration.m4dWorldToSphereSpace * sEyeConfiguration.v3dEyePosition;
    auto v3dSphereSpaceRayDirection = (
        sEyeConfiguration.m4dWorldToSphereSpace 
        * (sEyeConfiguration.v3dEyePosition + v3dRayDirection)
        - v3dSphereSpaceRayOrigin).normalized();

    auto r3dEyePhotonTracer = m_pVectorFactory->R3DFromOriginAndDirectionV3D(
        v3dSphereSpaceRayOrigin, v3dSphereSpaceRayDirection);

    auto ov3dSphereSpaceLensIntersection = m_pGeometry->IntersectWithSphereFromWithin(
        r3dEyePhotonTracer, s_sLensProxySphere);

    if (!ov3dSphereSpaceLensIntersection)
        return s_v2dErrorResult;

    const auto& v3dSphereSpaceLensIntersection = ov3dSphereSpaceLensIntersection.value();
    auto v3dSphereSpaceNormal = (
        s_sLensProxySphere.origin 
        - v3dSphereSpaceLensIntersection).normalized(); 

    v3dSphereSpaceNormal = (
        v3dSphereSpaceNormal.array() / s_v3dEllipsoidSphereWarp.array()).matrix().normalized();

    auto v3dWorldSpaceIntersection = sEyeConfiguration.m4dSphereToWorldSpace * v3dSphereSpaceLensIntersection;
    auto v3dWorldSpaceNormal = (
        sEyeConfiguration.m4dSphereToWorldSpace.linear()
        * v3dSphereSpaceNormal).normalized();

    auto ov3dReflectedEyePhotonDirection = m_pGeometry->Reflect(
        v3dRayDirection, v3dWorldSpaceNormal);

    if (!ov3dReflectedEyePhotonDirection)
        return s_v2dErrorResult;

    auto r3dWorldSpaceLensBounce = m_pVectorFactory->R3DFromOriginAndDirectionV3D(
        v3dWorldSpaceIntersection, ov3dReflectedEyePhotonDirection.value());

    auto ov3dWorldSpaceScreenIntersection =
        m_pGeometry->IntersectWithPlane(
            r3dWorldSpaceLensBounce,
            Plane3d(
                sEyeConfiguration.v3dScreenForward.normalized(),
                sEyeConfiguration.v3dScreenPosition));

    if (!ov3dWorldSpaceScreenIntersection)
        return s_v2dErrorResult;

    auto v3dScreenSpace = sEyeConfiguration.m4dWorldToScreenSpace * ov3dWorldSpaceScreenIntersection.value();
    auto v2dUV = m_pSpaceAdapter->FromUnityClipNearPlaneToUnityUV(v3dScreenSpace);
    auto v2dUnityConfigCorrected = 
        m_pVectorFactory->V2DFromArray({ 1, 1 }) 
        - m_pVectorFactory->V2DFromArray({ v2dUV.y(), v2dUV.x() });

    return v2dUnityConfigCorrected;
}
