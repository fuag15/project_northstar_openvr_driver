/*
    BridgeEngineTypes.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Bridge Engine SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

#pragma once

#include <ST/MathTypes.h>

namespace ST
{

    /** @brief Common fields for reporting certain information on the state of the
     * tracker. */
    struct TrackingStatus
    {
        /** Progress marker for pre-initialization of some tracking methods, esp.
         * depth-less modes. */
        int scaleInitializationPercent = 0;

        /** Progress marker for pre-initialization of tracking methods that use IMU data.
         */
        bool motionIsInitialized = false;

        /** When true, BridgeEngine is currently tracking. */
        bool isTracking = false;

        /** When true, BridgeEngine is paused internally and will not attempt to track. */
        bool isPaused = false;
    };

    /** @brief Return struct for predicted poses using the prediction API @see
     * BridgeEngine::predictCameraPose. */
    struct TrackerPrediction
    {
        /** When true, predictedWorldFromFrame will contain a valid pose. */
        bool couldPredict = false;

        /** The pose predicted at the given timestamp. */
        Matrix4 predictedWorldFromFrame = Matrix4::nan ();

        /** The linear velocity predicted at the given timestamp. */
        Vector3f linearVelocityInWorld = Vector3f (NAN, NAN, NAN);

        /** The angular velocity predicted at the given timestamp. */
        Vector3f angularVelocityInFrame = Vector3f (NAN, NAN, NAN);

        /** The linear acceleration predicted at the given timestamp. */
        Vector3f linearAccelerationInWorld = Vector3f (NAN, NAN, NAN);

        /** The angular acceleration predicted at the given timestamp. */
        Vector3f angularAccelerationInFrame = Vector3f (NAN, NAN, NAN);

        /** The timestamp provided to BridgeEngine::predictCameraPose. */
        double targetTimestamp = NAN;

        /** Currently not used. */
        double mostRecentGyroSample = NAN;
        /** Currently not used. */
        double mostRecentAccelSample = NAN;
        /** Currently not used. */
        double mostRecentCameraSample = NAN;
    };

    /** @brief The frame of reference for poses - which eye of a sensor should be used as
     * the basis. */
    enum class TrackerPoseFrameOfReference
    {
        /** The left-eye on the Structure Core unit, or the depth sensor. */
        StructureCoreDepth,

        /** The right-eye on the Structure Core unit, or the visible camera. */
        StructureCoreVisible,

        /** Currently not used. */
        ExternalColor,
    };

    /** @brief Hint about how the tracker is going to be used. */
    enum class AppUsageHint
    {
        /** Don't assume an AR/VR application - default settings applied. */
        None,

        /** Try to keep tracking as smooth as possible, avoiding sudden corrections.
            Only used with ModelFree tracking. */
        VirtualReality,

        /** Always return the most accurate estimate of camera pose, even if this can lead
           to small jumps after loop closures. Only used with ModelFree tracking. */
        AugmentedReality,
    };



};  // namespace ST
