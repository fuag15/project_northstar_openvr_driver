/*
    XRSession.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Perception Engine SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

#pragma once

#include <ST/CameraFrames.h>
#include <ST/Macros.h>
#include <ST/XRTypes.h>

#include <functional>


namespace ST
{

/** @brief Connects to the XRService and provides information from the tracking engine. */
struct ST_API XRSession
{
    /** @brief Returns the current timestamp in seconds from the XRService. */
    static double currentTime();

    /** @brief Returns true if the XRService is running. */
    static bool isTheServiceProcessCurrentlyRunning();

    /** @brief Returns true if there is another session running on this machine. */
    static bool isThereASessionRunningInAnyProcess();

    /** @brief Returns true if there is another session running in a separate process. */
    static bool isThereASessionRunningInAnotherProcess();

    /** @brief Constructor.
        @brief Session objects cannot be statically initialized and must be created AFTER the process entry point (main function) has been called.
    */
    XRSession();
    ~XRSession();

    /** @brief Connect to the XRService server.
        @return XRStatus. @see XRStatus
    */
    XRStatus connectToServer(const std::function<void()>& reconnectCbk = nullptr);

    /** @brief Disconnect from the server.
        @return XRStatus. @see XRStatus
    */
    XRStatus disconnectFromServer();

    /** @brief Tell the server to connect to the sensors and start tracking.
        @return XRStatus. @see XRStatus
    */
    XRStatus startTracking(const XRSessionSettings& settings);

    /** @brief Tell the server to stop tracking and disconnect from the sensors.
        @return XRStatus. @see XRStatus
    */
    XRStatus stopTracking();

    /** @brief Return whether the client is connected to the server. */
    bool isConnectedToServer() const;

    /** @brief Unimplemented. @see XRSession::predictWorldFromCameraPose
        @return XRStatus. @see XRStatus
    */
    XRStatus readWorldFromCameraPoses(XRFrameOfReference ref, XRPoseUpdateFunction updateFunction);
    
    /** @brief Retrieve the last tracked pose from the XRService.
        @param ref The frame of reference of the returned pose.
        @param pose The last tracked pose.
        @param minTimestamp The timestamp of the last retrieved pose, in seconds.
        @return XRStatus. @see XRStatus
    */
    XRStatus readLastWorldFromCameraPose(XRFrameOfReference ref, XRPose& pose, double minTimestamp);

    /** @brief Get the predicted pose at the given timestamp from the XRService.
        @param ref The frame of reference of the returned pose.
        @param timestamp The timestamp to predict to, in seconds.
        @return XRStatus. @see XRStatus
        @see BridgeEngine::predictCameraPose
    */
    XRStatus predictWorldFromCameraPose(XRFrameOfReference ref, double timestamp, XRPose& pose);

    /** @brief Retrieve the local sparse map (as a point cloud) of the current scene from the XRService.
        @param pointMesh Reference to an ST::Mesh class to hold the point cloud sparse map data.
        @param lineMesh Reference to an ST::Mesh class to hold the line sparse map data.
        @return XRStatus. @see XRStatus
        @see BridgeEngine::getSparseMap
    */
    XRStatus getSparseMap(Mesh& pointMesh, Mesh& lineMesh) const;


    /** @brief Unimplemented.
        @return XRStatus. @see XRStatus
        @see BridgeEngine::getDenseMap
    */
    XRStatus getDenseMap(Mesh& denseMap) const;

    /** @brief Temporarily pauses tracking. Does not use incoming frames but keeps the state
        @brief of the tracker intact. Call resumeTracking to continue tracking.
        @return XRStatus. @see XRStatus
    */
    XRStatus pauseTracking() const;

    /** @brief Resumes tracking from a paused state. Does nothing if not currently paused.
        @return XRStatus. @see XRStatus
    */
    XRStatus resumeTracking() const;

    /** @brief Reset the current state of tracking.
        @brief Tracking becomes completely re-initialized when resetTracking is called.
        @return XRStatus. @see XRStatus
        @see BridgeEngine::resetTracking
    */
    XRStatus resetTracking();

    /** @brief Convenience method to set the exposure and gain of the visible stream from Structure Core.
        @param exposure The exposure of the visible stream. Can be a valid floating point number between 0 and 0.03.
        @param gain The gain of the visible stream. Can be a valid floating point number between 1 and 8.
        @param isRelativeDelta Whether the provided values should be treated as differences to the current ones.
        @return XRStatus. @see XRStatus
        @see XRNumericalProperty to retrieve the current values.
    */
    XRStatus setVisibleExposureAndGain(float exposure, float gain, bool isRelativeDelta);

    /** @brief Provides the ability to specify running a certain type of application.
        @brief Modifies internal tracking and mapping settings to better suit the application type.
        @return XRStatus. @see XRStatus
    */
    XRStatus setAppUsageHint(XRAppUsageHint usageHint);

    /** @brief Returns true if we are connected and the tracker is running. */
    bool isTrackingRunning() const;

    /** @brief When the XRStatus return of a function in this service shows an error,
        @brief this function can be used to retrieve a more detailed description of the error.

        Example usage:
        @code{.cpp}
        XRStatus status = mySession.startSession();
        if (status != XRStatus::Good) {
            std::printf("[ERROR] Could not start the XRSession. Reason: (%i) %s\n",
                        status, mySession.lastSessionError());
            return EXIT_FAILURE;
        }
        ...
        @endcode
        @return UTF8 string describing the last error that happened.
    */
    const char* lastSessionError() const;

    /** @brief Retrieve the last streamed visible image from Structure Core.
    @return XRStatus. @see XRStatus
    */
    XRStatus readLastVisibleFrame(ColorFrame& visibleFrame, double minTimestamp);

    /** @brief Retrieve the last streamed depth image from Structure Core.
        @return XRStatus. @see XRStatus
    */
    XRStatus readLastDepthFrame(DepthFrame& depthFrame, double minTimestamp);


    ST_DECLARE_OPAQUE_INTERNALS(XRSession);
};

} // ST namespace
