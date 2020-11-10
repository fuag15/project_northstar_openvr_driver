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

    return sEyeConfiguration;
}

Vector4d northstar::driver::C2DCalibratedOptics::GetEyeProjectionLRTB(const vr::EVREye& eEye) {
    return Vector4d{ 1.0, 1.0, 1.0, 1.0 };
}

Vector2d northstar::driver::C2DCalibratedOptics::EyeUVToScreenUV(const vr::EVREye& eEye, const Vector2d& v2dTargetEyeUV) {
    return Vector2d{ 0.0, 0.0 };
}
