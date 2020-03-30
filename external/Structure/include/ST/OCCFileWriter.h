/*
    OCCFileWriter.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Structure SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

#pragma once

#include <ST/CaptureSession.h>

namespace ST
{
/** @brief Utility class to exist with creating OCC files from streamed data.
    An OCC file is a bespoke Occipital file format for recording streamed data
    from a sensor, allowing Occipital to playback recorded data streams in house
    as if the same hardware was used.
*/
struct ST_API OCCFileWriter
{
    /** @brief Constructor. */
    OCCFileWriter();
    ~OCCFileWriter();

    /** @brief Initialize OCC recording and create the initial OCC file.
        @param path The fullpath of where to store the OCC file.
        @param useH264 Not available on Windows. If true, images will be saved using H264 compression.
        @return True, if the OCC file was created and OCC recording can happen. Otherwise, false.
    */
    bool startWritingToFile(const char* path, bool useH264 = false);

    /** @brief Wait for all queued data to be written to the file and flush to the drive. */
    void finalizeWriting();

    /** @brief Utility function to write any type of sample streamed from CaptureSession to the OCC. */
    void writeCaptureSample(const CaptureSessionSample& sample);

    /** @brief Write the provided AccelerometerEvent to the OCC file. */
    void writeAccelerometerEvent(const AccelerometerEvent& accelerometerEvent);

    /** @brief Write the provided GyroscopeEvent to the OCC file. */
    void writeGyroscopeEvent(const GyroscopeEvent& gyroscopeEvent);

    /** @brief Write the provided InfraredFrame to the OCC file. */
    void writeInfraredFrame(const InfraredFrame& infraredFrame);

    /** @brief Write the provided DepthFrame to the OCC file. */
    void writeDepthFrame(const DepthFrame& depthFrame);

    /** @brief Write the provided ColorFrame (visible data) to the OCC file. */
    void writeVisibleFrame(const ColorFrame& visibleFrame);

    /** @brief Write the provided ColorFrame to the OCC file. */
    void writeColorFrame(const ColorFrame& colorFrame);

    /** @brief Write the provided synchronized frames to the OCC file.
        @brief This assumes the frames have been sent from CaptureSession together.
        @brief Any of the frames can be nullptr. If so, the parameter is ignored. This function
        @brief will only write valid frame data to the OCC.
    */
    void writeSynchronizedFrames(
        const InfraredFrame* infraredFrameOrNull,
        const DepthFrame* depthFrameOrNull,
        const ColorFrame* visibleFrameOrNull,
        const ColorFrame* colorFrameOrNull
    );

    /** @brief Write the provided pose to the OCC file.
        @param cameraPose The current position of the camera.
        @param timestamp The timestamp in seconds when the pose was either tracked or provided.
    */
    void writeCameraPose(const Matrix4& cameraPose, double timestamp);


    /** @brief Check if the writer is active or not
        @return True, if a file is being writen to
    */
    bool isWriting();

    ST_DECLARE_OPAQUE_INTERNALS(OCCFileWriter);
};

} // ST namespace
