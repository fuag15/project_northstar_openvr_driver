/*
    C_XRTypes.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Bridge Engine SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

/** @brief C_XRTypes.h contains overloaded C# bindings for the XR types used in BridgeEngine.
    @see XRTypes.h
*/

#pragma once

#include <ST/Macros.h>
#include <ST/C_MathTypes.h>
#include <ST/Mesh.h>
#include <ST/CameraFrames.h>
#include <ST/BridgeEngineTypes.h>


ST_PLAIN_C_BEGIN

//------------------------------------------------------------------------------

#define XR_MAX_FLOATS_IN_NUMBER_PROPERTY 16

enum ST_XRNumericalProperty
{
    ST_XRNumericalProperty_VisibleExposureAndGain,
    ST_XRNumericalProperty_AppUsageHint,
    ST_XRNumericalProperty_RoomLayoutEstimationEnabled,

    ST_XRNumericalProperty_HowMany,
};

//------------------------------------------------------------------------------

enum ST_XRFrameOfReference
{
    ST_XRFrameOfReference_Invalid = -1,

    ST_XRFrameOfReference_VisibleCamera,
    ST_XRFrameOfReference_DepthCamera,
    ST_XRFrameOfReference_StereoCamera,

    ST_XRFrameOfReference_HowMany,
};

//------------------------------------------------------------------------------

enum ST_XRTrackingMode
{
    ST_XRTrackingMode_RGBD,
    ST_XRTrackingMode_Monocular,
    ST_XRTrackingMode_Stereo,
};

//------------------------------------------------------------------------------

enum ST_XRMappingMode
{
    ST_XRMappingMode_None,
    ST_XRMappingMode_Sparse,
    ST_XRMappingMode_SparseAndDense,
};

//------------------------------------------------------------------------------

enum ST_XRAppUsageHint
{
    ST_XRAppUsageHint_None,
    ST_XRAppUsageHint_VirtualReality,
    ST_XRAppUsageHint_AugmentedReality,
};

//------------------------------------------------------------------------------

enum ST_XRStatus
{
    ST_XRStatus_Good,
    ST_XRStatus_NotImplemented,
    ST_XRStatus_FailedToInitialize,
    ST_XRStatus_ClientNotConnected,
    ST_XRStatus_DataReturnedNotValid,
    ST_XRStatus_DataNotYetAvailable,
    ST_XRStatus_Error,
};

//------------------------------------------------------------------------------

struct ST_API ST_XRSessionSettings
{
    ST_XRTrackingMode trackingMode;
    ST_XRMappingMode  mappingMode;
    ST_Matrix4        initialPose;
};

ST_API void ST_CDECL ST_XRSessionSettings_init(ST_XRSessionSettings* settings);

//------------------------------------------------------------------------------

struct ST_API ST_XRTrackingStatus
{
    bool cameraSuccessfullyTracked = false;
    bool motionIsInitialized = false;
    float scaleConfidence = NAN;
};

ST_API void ST_CDECL ST_XRTrackingStatus_init(ST_XRTrackingStatus* status);

//------------------------------------------------------------------------------

struct ST_API ST_XRPose
{
    ST_XRTrackingStatus status;
    double              timestamp;
    double              mostRecentAccelSampleTimestamp;
    double              mostRecentGyroSampleTimestamp;
    double              mostRecentTrackedFrameTimestamp;
    ST_Matrix4          matrix;
    float linearVelocity[3];
    float angularVelocity[3];
    float linearAcceleration[3];
    float angularAcceleration[3];
};

ST_API void ST_CDECL ST_XRPose_init    (ST_XRPose* pose);
ST_API bool ST_CDECL ST_XRPose_isValid (ST_XRPose* pose);

typedef void (*ST_XRPoseUpdateFunction) (ST_XRPose*);

//------------------------------------------------------------------------------


struct ST_Mesh : ST::Mesh {};

ST_API ST_Mesh*               ST_CDECL ST_Mesh_new            ();
ST_API void                   ST_CDECL ST_Mesh_delete         (ST_Mesh* mesh);
ST_API const float*           ST_CDECL ST_Mesh_getVertexData  (ST_Mesh* mesh);
ST_API int                    ST_CDECL ST_Mesh_getVertexCount (ST_Mesh* mesh);
ST_API const unsigned short*  ST_CDECL ST_Mesh_getIndexData   (ST_Mesh* mesh);
ST_API int                    ST_CDECL ST_Mesh_getIndexCount  (ST_Mesh* mesh);
ST_API const unsigned short*  ST_CDECL ST_Mesh_getLineData    (ST_Mesh* mesh);
ST_API int                    ST_CDECL ST_Mesh_getLineCount   (ST_Mesh* mesh);

//------------------------------------------------------------------------------

struct ST_ColorFrame : ST::ColorFrame {};

ST_API ST_ColorFrame*       ST_CDECL ST_ColorFrame_new      ();
ST_API void                 ST_CDECL ST_ColorFrame_delete   (ST_ColorFrame* frame);
ST_API const unsigned char* ST_CDECL ST_ColorFrame_getImage (ST_ColorFrame* frame);

//------------------------------------------------------------------------------


ST_PLAIN_C_END
