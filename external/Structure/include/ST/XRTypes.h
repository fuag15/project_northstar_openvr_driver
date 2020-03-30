/*
    XRTypes.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Perception Engine SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

#pragma once

#include <ST/Macros.h>
#include <ST/MathTypes.h>
#include <ST/C_XRTypes.h>

namespace ST
{

//------------------------------------------------------------------------------

// forward declares
struct Mesh;

//------------------------------------------------------------------------------

/** @brief Specifies which property to request/set. */
enum class XRNumericalProperty : int32_t
{
    /** @brief Exposure and gain values of the visible camera.
        When setting it, you need to provide 3 values: exposure, gain, and whether it is a relative delta or an absolute value (bool 0 or 1)
        When getting them, the two float values for exposure and gain will be returned.
        @see XRSession::setVisibleExposureAndGain for a convenience wrapper.
    */
    VisibleExposureAndGain      = ST_XRNumericalProperty_VisibleExposureAndGain,

    /** @brief Provides the ability to specify running a certain type of application.
        Modifies internal tracking and mapping settings to better suit the application type.
        The associated parameters should be one of the XRAppUsageHint values.
    */
    AppUsageHint                = ST_XRNumericalProperty_AppUsageHint,

    /** @brief Specifies whether the room layout estimation shoud keep running. */
    RoomLayoutEstimationEnabled = ST_XRNumericalProperty_RoomLayoutEstimationEnabled,

    HowMany                     = ST_XRNumericalProperty_HowMany,
};

/** @brief Wrapper for BridgeEngine::TrackerPoseFrameOfReference */
enum class XRFrameOfReference
{
    Invalid       = ST_XRFrameOfReference_Invalid,

    /** Structure Core visible frame of reference. */
    VisibleCamera = ST_XRFrameOfReference_VisibleCamera,

    /** Structure Core depth frame of reference. */
    DepthCamera   = ST_XRFrameOfReference_DepthCamera,

    /** Stereo sensor frame of reference. */
    StereoCamera  = ST_XRFrameOfReference_StereoCamera,

    HowMany       = ST_XRFrameOfReference_HowMany,



    // Default = N/A, it is not safe to assume a default value for this enum
};

/** @brief Specifies which mode of tracking to run in the XRService. */
enum class XRTrackingMode
{
    Invalid = -1,

    RGBD = ST_XRTrackingMode_RGBD,
    Monocular = ST_XRTrackingMode_Monocular,
    Stereo = ST_XRTrackingMode_Stereo,

    HowMany,
    Default = RGBD,
};

/** @brief Specifies which mode of mapping to run in the XRService. */
enum class XRMappingMode
{
    Invalid = -1,

    /** Do not run mapping. */
    None           = ST_XRMappingMode_None,

    /** Only map and collect the sparse map. */
    Sparse         = ST_XRMappingMode_Sparse,

    /** Map and collect both the sparse and dense map. */
    SparseAndDense = ST_XRMappingMode_SparseAndDense,

    HowMany,
    Default = None,
};

/** @brief Provides the ability to specify running a certain type of application using the XR API. @see AppUsageHint */
enum class XRAppUsageHint
{
    Invalid = -1,

    /** Don't assume an AR/VR application - default settings applied. */
    None             = ST_XRAppUsageHint_None,

    /** Modifies internal settings to better suit Virtual Reality applications. (e.g. in-headset) */
    VirtualReality   = ST_XRAppUsageHint_VirtualReality,

    /** Modifies internal settings to better suit Augmented Reality applications. (e.g. mobile apps) */
    AugmentedReality = ST_XRAppUsageHint_AugmentedReality,

    HowMany,
    Default = None,
};

/** @brief Return status describing the success of functions in the XR API. */
enum class XRStatus
{
    Invalid = -1,

    /** Success! */
    Good                 = ST_XRStatus_Good,

    /** Function is not implemented. */
    NotImplemented       = ST_XRStatus_NotImplemented,

    /** Function failed to initialize, or failed due to lack of initialization. */
    FailedToInitialize   = ST_XRStatus_FailedToInitialize,

    /** Function failed to perform an action because a session is not running. */
    ClientNotConnected   = ST_XRStatus_ClientNotConnected,

    /** Function ran successfully, but the output should not be used for some reason. */
    DataReturnedNotValid = ST_XRStatus_DataReturnedNotValid,

    /** Data requested is not yet ready for retrieval. (e.g. requesting a pose before tracking is running) */
    DataNotYetAvailable  = ST_XRStatus_DataNotYetAvailable,

    /** Non-descript fallback for other errors. Shouldn't be used too much. */
    Error                = ST_XRStatus_Error,
};

/** @brief Struct to hold initialization parameters for XRSession. */
struct ST_API XRSessionSettings
{
    /** Specifies which mode of tracking to run in the XRService. */
    XRTrackingMode trackingMode = XRTrackingMode::Default;

    /** Specifies which mode of mapping to run in the XRService. */
    XRMappingMode  mappingMode  = XRMappingMode::Default;

    /** The initial pose to force when tracking starts in the XRService. */
    Matrix4        initialPose  = Matrix4::identity();

    /** @brief Attempts to read XRSession presaved settings from "[Documents]/CaptureSessionSettings.json". */
    void readSavedXRSessionSettings();

    /** @brief Writes the current state of the struct into "[Documents]CaptureSessionSettings.json". */
    void persistXRSessionSettings();
};

/** @brief Wrapper for BridgeEngine::TrackingStatus */
struct ST_API XRTrackingStatus
{
    bool cameraSuccessfullyTracked = false;
    bool motionIsInitialized = false;
    float  scaleInitializationPercent = 0;
};

/** @brief Container struct for tracked poses returned from the XRService. */
struct ST_API XRPose
{
    /** @brief Returns true if this pose is valid. Otherwise, false. */
    bool isValid () const { return !std::isnan(timestamp); }

    /** @brief The current state of tracking. */
    XRTrackingStatus status;

    /** @brief The timestamp in seconds when this pose was tracked. */
    double           timestamp = NAN;

    /** @brief The timestamp in seconds of the last accelerometer sample used to compute the pose. */
    double mostRecentAccelSampleTimestamp = NAN;

    /** @brief The timestamp in seconds of the last gyroscope sample used to compute the pose. */
    double mostRecentGyroSampleTimestamp = NAN;

    /** @brief The timestamp in seconds of the last camera frame used to compute the pose. */
    double mostRecentTrackedFrameTimestamp = NAN;

    /** @brief The current position of the camera. */
    Matrix4          matrix = Matrix4::nan();

    /** The linear velocity predicted at the given timestamp. */
    float linearVelocity[3] = { NAN, NAN, NAN };

    /** The angular velocity predicted at the given timestamp. */
    float angularVelocity[3] = { NAN, NAN, NAN };

    /** The linear acceleration predicted at the given timestamp. */
    float linearAcceleration[3] = { NAN, NAN, NAN };

    /** The angular acceleration predicted at the given timestamp. */
    float angularAcceleration[3] = { NAN, NAN, NAN };
};

//------------------------------------------------------------------------------

using XRPoseUpdateFunction = void (*) (const XRPose&);


//------------------------------------------------------------------------------

} // ST namespace
