#include "driver/3DCalibratedOptics.hpp"

using namespace northstar::math::types;
using namespace northstar::driver::settings::keys;

// TODO: Make this 2 instances and choose on instantiation
northstar::driver::C3DCalibratedOptics::C3DCalibratedOptics(
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

northstar::driver::C3DCalibratedOptics::SEyeConfiguration northstar::driver::C3DCalibratedOptics::LoadConfigFromEye(const vr::EVREye& eEye) {
    auto sEyeKey = eEye == vr::Eye_Left ? eye3D::k_svRootLeft : eye3D::k_svRootRight;
    SEyeConfiguration sEyeConfiguration;
    sEyeConfiguration.sEllipsisAxis.dMajor = m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEllipseMajorAxis.data());
    sEyeConfiguration.sEllipsisAxis.dMinor = m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEllipseMinorAxis.data());
    sEyeConfiguration.v3dScreenForward =
        m_pVectorFactory->V3DFromArray(
            { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenForwardX.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenForwardY.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenForwardZ.data()) });

    sEyeConfiguration.v3dScreenPosition =
        m_pVectorFactory->V3DFromArray(
            { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenPositionX.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenPositionY.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svScreenPositionZ.data()) });

    sEyeConfiguration.v3dEyePosition =
        m_pVectorFactory->V3DFromArray(
            { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyePositionX.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyePositionY.data())
            , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyePositionZ.data()) });

    sEyeConfiguration.qdEyeRotation =
        Quaterniond(
            m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyeRotationW.data()),
            m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyeRotationX.data()),
            m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyeRotationY.data()),
            m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svEyeRotationZ.data()));

    sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB =
        m_pWorldAdapter->FromUnityProjectionExtentsLRTBToOpenVRProjectionExtentsLRTB(
            m_pVectorFactory->V4DFromXYZWArray(
                { m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svCameraProjectionX.data())
                , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svCameraProjectionY.data())
                , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svCameraProjectionZ.data())
                , m_pVRSettings->GetFloat(sEyeKey.data(), eye3D::k_svCameraProjectionW.data()) }));

    sEyeConfiguration.m4dSphereToWorldSpace =
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

    sEyeConfiguration.m4dWorldToScreenSpace =
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

    sEyeConfiguration.m4dWorldToSphereSpace = sEyeConfiguration.m4dSphereToWorldSpace.inverse();
    sEyeConfiguration.m4dCameraProjection = 
        m_pMatrixFactory->OpenGLProjectionFromFrustumExtents(
            sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB.x(),
            sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB.y(),
            sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB.z(),
            sEyeConfiguration.v4dCameraProjectionFrustumExtentsLRTB.w());

    auto m4dEyeTranslation = m_pMatrixFactory->TranslationFromVector3d(sEyeConfiguration.v3dEyePosition);
    auto m4dEyeToWorld = m4dEyeTranslation * sEyeConfiguration.qdEyeRotation;
    m4dEyeToWorld.matrix().col(2) *= -1.0;
    sEyeConfiguration.m4dClipToWorld = m4dEyeToWorld * sEyeConfiguration.m4dCameraProjection.inverse();
    return sEyeConfiguration;
}

Vector4d northstar::driver::C3DCalibratedOptics::GetEyeProjectionLRTB(const vr::EVREye& eEye) {
    return m_umEyeConfigs.at(eEye).v4dCameraProjectionFrustumExtentsLRTB;
}

Vector2d northstar::driver::C3DCalibratedOptics::EyeUVToScreenUV(const vr::EVREye& eEye, const Vector2d& v2dTargetEyeUV) {
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
Vector2d northstar::driver::C3DCalibratedOptics::IterativeGradientUVWarpSolve(
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
Vector2d northstar::driver::C3DCalibratedOptics::ReverseProjectEyeUVToDisplayUV(const vr::EVREye& eEye, const Vector2d& v2dEyeUV) {
    static const auto s_v2dErrorResult = m_pVectorFactory->V2DFromArray(x_daIterativeSolverErrorResult);
    static const SSphere s_sLensProxySphere = {
        m_pVectorFactory->V3DFromArray(x_daLensProxySphereOrigin),
        x_dLensProxySphereRadius };

    auto sEyeConfiguration = m_umEyeConfigs.at(eEye);
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
