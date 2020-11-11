#pragma once

#include <string_view>

namespace northstar {
    namespace driver {
        namespace settings {
            namespace keys {
                namespace debug {
                    static constexpr std::string_view k_svRoot = "debug";

                    /* from Controller */
                    static constexpr std::string_view k_svUseDebugTrackerConfig = "useDebugTrackerConfig";
                    static constexpr std::string_view k_svUseDebugBasePose = "useDebugBasePose";

                    /* from HMD */
                    static constexpr std::string_view k_svUseFakeScreenConfig = "useFakeScreenConfig";
                    static constexpr std::string_view k_svUseFakeProjection = "useFakeProjection";
                    static constexpr std::string_view k_svUseFakeWarp = "useFakeWarp";
                    static constexpr std::string_view k_svUseFakeTracking = "useFakeTracking";

                    /* from Server */
                    static constexpr std::string_view k_svUseControllerDebugMode = "useControllerDebugMode";
                    static constexpr std::string_view k_svShouldBlockStandbyMode = "shouldBlockStandbyMode";
                }

                namespace configuration {
                    static constexpr std::string_view k_svRoot = "configuration";

                    /* from Controller */
                    static constexpr std::string_view k_svUseRenderModel = "useRenderModel";

                    /* from Server */
                    static constexpr std::string_view k_svEnableControllers = "enableControllers";

                    /* from Optics */
                    static constexpr std::string_view k_svUse2DCalibrationData = "use2DCalibrationData";
                }

                namespace display {
                    static constexpr std::string_view k_svRoot = "display";
                    static constexpr std::string_view k_svOriginX = "originX";
                    static constexpr std::string_view k_svOriginY = "originY";
                    static constexpr std::string_view k_svWidth = "width";
                    static constexpr std::string_view k_svHeight = "height";
                    static constexpr std::string_view k_svRenderWidth = "renderWidth";
                    static constexpr std::string_view k_svRenderHeight = "renderHeight";
                    static constexpr std::string_view k_svFrequency = "frequency";
                    static constexpr std::string_view k_svIPD = "ipd";
                    static constexpr std::string_view k_svPhotonLatency = "photonLatency";
                }

                namespace eye2D {
                    static constexpr std::string_view k_svRootLeft = "leftEye2DCalibrationData";
                    static constexpr std::string_view k_svRootRight = "rightEye2DCalibrationData";
                    static constexpr std::string_view k_svRootSharedConfig = "shared2DCalibrationData";

                    static constexpr std::string_view k_svBaseline = "baseline";

                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_0 = "UVToRectilinearXAxisCoefficient_0";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_1 = "UVToRectilinearXAxisCoefficient_1";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_2 = "UVToRectilinearXAxisCoefficient_2";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_3 = "UVToRectilinearXAxisCoefficient_3";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_4 = "UVToRectilinearXAxisCoefficient_4";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_5 = "UVToRectilinearXAxisCoefficient_5";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_6 = "UVToRectilinearXAxisCoefficient_6";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_7 = "UVToRectilinearXAxisCoefficient_7";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_8 = "UVToRectilinearXAxisCoefficient_8";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_9 = "UVToRectilinearXAxisCoefficient_9";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_10 = "UVToRectilinearXAxisCoefficient_10";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_11 = "UVToRectilinearXAxisCoefficient_11";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_12 = "UVToRectilinearXAxisCoefficient_12";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_13 = "UVToRectilinearXAxisCoefficient_13";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_14 = "UVToRectilinearXAxisCoefficient_14";
                    static constexpr std::string_view k_svUVToRectilinearXAxisCoefficient_15 = "UVToRectilinearXAxisCoefficient_15";

                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_0 = "UVToRectilinearYAxisCoefficient_0";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_1 = "UVToRectilinearYAxisCoefficient_1";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_2 = "UVToRectilinearYAxisCoefficient_2";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_3 = "UVToRectilinearYAxisCoefficient_3";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_4 = "UVToRectilinearYAxisCoefficient_4";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_5 = "UVToRectilinearYAxisCoefficient_5";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_6 = "UVToRectilinearYAxisCoefficient_6";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_7 = "UVToRectilinearYAxisCoefficient_7";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_8 = "UVToRectilinearYAxisCoefficient_8";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_9 = "UVToRectilinearYAxisCoefficient_9";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_10 = "UVToRectilinearYAxisCoefficient_10";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_11 = "UVToRectilinearYAxisCoefficient_11";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_12 = "UVToRectilinearYAxisCoefficient_12";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_13 = "UVToRectilinearYAxisCoefficient_13";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_14 = "UVToRectilinearYAxisCoefficient_14";
                    static constexpr std::string_view k_svUVToRectilinearYAxisCoefficient_15 = "UVToRectilinearYAxisCoefficient_15";

                    static constexpr std::string_view k_svCameraProjection_00 = "cameraProjection_e00";
                    static constexpr std::string_view k_svCameraProjection_01 = "cameraProjection_e01";
                    static constexpr std::string_view k_svCameraProjection_02 = "cameraProjection_e02";
                    static constexpr std::string_view k_svCameraProjection_03 = "cameraProjection_e03";
                    static constexpr std::string_view k_svCameraProjection_10 = "cameraProjection_e10";
                    static constexpr std::string_view k_svCameraProjection_11 = "cameraProjection_e11";
                    static constexpr std::string_view k_svCameraProjection_12 = "cameraProjection_e12";
                    static constexpr std::string_view k_svCameraProjection_13 = "cameraProjection_e13";
                    static constexpr std::string_view k_svCameraProjection_20 = "cameraProjection_e20";
                    static constexpr std::string_view k_svCameraProjection_21 = "cameraProjection_e21";
                    static constexpr std::string_view k_svCameraProjection_22 = "cameraProjection_e22";
                    static constexpr std::string_view k_svCameraProjection_23 = "cameraProjection_e23";
                    static constexpr std::string_view k_svCameraProjection_30 = "cameraProjection_e30";
                    static constexpr std::string_view k_svCameraProjection_31 = "cameraProjection_e31";
                    static constexpr std::string_view k_svCameraProjection_32 = "cameraProjection_e32";
                    static constexpr std::string_view k_svCameraProjection_33 = "cameraProjection_e33";

                    static constexpr std::string_view k_svCameraProjectionX = "cameraProjection_x";
                    static constexpr std::string_view k_svCameraProjectionY = "cameraProjection_y";
                    static constexpr std::string_view k_svCameraProjectionZ = "cameraProjection_z";
                    static constexpr std::string_view k_svCameraProjectionW = "cameraProjection_w";
                }

                namespace eye3D {
                    static constexpr std::string_view k_svRootLeft = "leftEye3DCalibrationData";
                    static constexpr std::string_view k_svRootRight = "rightEye3DCalibrationData";

                    static constexpr std::string_view k_svEllipseMinorAxis = "ellipseMinorAxis";
                    static constexpr std::string_view k_svEllipseMajorAxis = "ellipseMajorAxis";

                    static constexpr std::string_view k_svScreenForwardX = "screenForward_x";
                    static constexpr std::string_view k_svScreenForwardY = "screenForward_y";
                    static constexpr std::string_view k_svScreenForwardZ = "screenForward_z";

                    static constexpr std::string_view k_svScreenPositionX = "screenPosition_x";
                    static constexpr std::string_view k_svScreenPositionY = "screenPosition_y";
                    static constexpr std::string_view k_svScreenPositionZ = "screenPosition_z";

                    static constexpr std::string_view k_svEyePositionX = "eyePosition_x";
                    static constexpr std::string_view k_svEyePositionY = "eyePosition_y";
                    static constexpr std::string_view k_svEyePositionZ = "eyePosition_z";

                    static constexpr std::string_view k_svEyeRotationX = "eyeRotation_x";
                    static constexpr std::string_view k_svEyeRotationY = "eyeRotation_y";
                    static constexpr std::string_view k_svEyeRotationZ = "eyeRotation_z";
                    static constexpr std::string_view k_svEyeRotationW = "eyeRotation_w";

                    static constexpr std::string_view k_svCameraProjectionX = "cameraProjection_x";
                    static constexpr std::string_view k_svCameraProjectionY = "cameraProjection_y";
                    static constexpr std::string_view k_svCameraProjectionZ = "cameraProjection_z";
                    static constexpr std::string_view k_svCameraProjectionW = "cameraProjection_w";

                    static constexpr std::string_view k_svSphereToWorldSpace_00 = "sphereToWorldSpace_e00";
                    static constexpr std::string_view k_svSphereToWorldSpace_01 = "sphereToWorldSpace_e01";
                    static constexpr std::string_view k_svSphereToWorldSpace_02 = "sphereToWorldSpace_e02";
                    static constexpr std::string_view k_svSphereToWorldSpace_03 = "sphereToWorldSpace_e03";
                    static constexpr std::string_view k_svSphereToWorldSpace_10 = "sphereToWorldSpace_e10";
                    static constexpr std::string_view k_svSphereToWorldSpace_11 = "sphereToWorldSpace_e11";
                    static constexpr std::string_view k_svSphereToWorldSpace_12 = "sphereToWorldSpace_e12";
                    static constexpr std::string_view k_svSphereToWorldSpace_13 = "sphereToWorldSpace_e13";
                    static constexpr std::string_view k_svSphereToWorldSpace_20 = "sphereToWorldSpace_e20";
                    static constexpr std::string_view k_svSphereToWorldSpace_21 = "sphereToWorldSpace_e21";
                    static constexpr std::string_view k_svSphereToWorldSpace_22 = "sphereToWorldSpace_e22";
                    static constexpr std::string_view k_svSphereToWorldSpace_23 = "sphereToWorldSpace_e23";

                    static constexpr std::string_view k_svWorldToScreenSpace_00 = "worldToScreenSpace_e00";
                    static constexpr std::string_view k_svWorldToScreenSpace_01 = "worldToScreenSpace_e01";
                    static constexpr std::string_view k_svWorldToScreenSpace_02 = "worldToScreenSpace_e02";
                    static constexpr std::string_view k_svWorldToScreenSpace_03 = "worldToScreenSpace_e03";
                    static constexpr std::string_view k_svWorldToScreenSpace_10 = "worldToScreenSpace_e10";
                    static constexpr std::string_view k_svWorldToScreenSpace_11 = "worldToScreenSpace_e11";
                    static constexpr std::string_view k_svWorldToScreenSpace_12 = "worldToScreenSpace_e12";
                    static constexpr std::string_view k_svWorldToScreenSpace_13 = "worldToScreenSpace_e13";
                    static constexpr std::string_view k_svWorldToScreenSpace_20 = "worldToScreenSpace_e20";
                    static constexpr std::string_view k_svWorldToScreenSpace_21 = "worldToScreenSpace_e21";
                    static constexpr std::string_view k_svWorldToScreenSpace_22 = "worldToScreenSpace_e22";
                    static constexpr std::string_view k_svWorldToScreenSpace_23 = "worldToScreenSpace_e23";
                }

                namespace leapMotion {
                    static constexpr std::string_view k_svRoot = "leapTrackerOdometryOrigin";
                    static constexpr std::string_view k_svPositionX = "position_x";
                    static constexpr std::string_view k_svPositionY = "position_y";
                    static constexpr std::string_view k_svPositionZ = "position_z";
                    static constexpr std::string_view k_svRotationX = "rotation_x";
                    static constexpr std::string_view k_svRotationY = "rotation_y";
                    static constexpr std::string_view k_svRotationZ = "rotation_z";
                    static constexpr std::string_view k_svRotationW = "rotation_w";
                }
            }

            namespace values {
                namespace icon {
                    static constexpr std::string_view k_svDeviceOff = "{sample}/icons/headset_sample_status_off.png";
                    static constexpr std::string_view k_svDeviceSearching = "{sample}/icons/headset_sample_status_searching.gif";
                    static constexpr std::string_view k_svDeviceSearchingAlert = "{sample}/icons/headset_sample_status_searching_alert.gif";
                    static constexpr std::string_view k_svDeviceReady = "{sample}/icons/headset_sample_status_ready.png";
                    static constexpr std::string_view k_svDeviceReadyAlert = "{sample}/icons/headset_sample_status_ready_alert.png";
                    static constexpr std::string_view k_svDeviceNotReady = "{sample}/icons/headset_sample_status_error.png";
                    static constexpr std::string_view k_svDeviceStandby = "{sample}/icons/headset_sample_status_standby.png";
                    static constexpr std::string_view k_svDeviceAlertLow = "{sample}/icons/headset_sample_status_ready_low.png";
                }

                namespace paths {
                    static constexpr std::string_view k_svInputProfilePath = "{northstar}/input/northstar_profile.json";
                    static constexpr std::string_view k_svTriggerValuePath = "/input/trigger/value";
                    static constexpr std::string_view k_svAClickPath = "/input/a/click";
                    static constexpr std::string_view k_svBClickPath = "/input/b/click";
                    static constexpr std::string_view k_svSystemClickPath = "/input/system/click";
                    static constexpr std::string_view k_svSkeletalInputPathNameLeft = "/input/skeleton/left";
                    static constexpr std::string_view k_svSkeletalInputPathNameRight = "/input/skeleton/right";
                    static constexpr std::string_view k_svSkeletalInputPathLeft = "/skeleton/hand/left";
                    static constexpr std::string_view k_svSkeletalInputPathRight = "/skeleton/hand/right";
                    static constexpr std::string_view k_svSkeletalInputPoseBasePath = "/pose/raw";
                }

                namespace driverConfiguration {
                    static constexpr uint64_t k_uiCurrentUniverseID = 2;
                    static constexpr int32_t k_unBoneCount = 31;
                }
            }
        }
    }
}