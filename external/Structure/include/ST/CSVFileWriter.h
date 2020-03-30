/*
    CSVFileWriter.h

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

/** @brief Utility class used to help with writing out tracked poses to a CSV file. */
struct ST_API CSVFileWriter
{
    /** @brief The output file format of the CSV. */
    enum class OutputFormat
    {
        /** Format structured as: [TS in Seconds, mat4x4 as 16 floats], Comma-separated. */
        TimestampInSeconds_PoseAsMatrix4,

        /** Format structured as: [TS in Milliseconds, rot in ZYX-euler as 3 floats, translation as 3 floats]. Space separated. */
        TimestampInMilliseconds_PoseAsVector6,

        /** Default defined for brevity */
        Default = TimestampInSeconds_PoseAsMatrix4
    };

    /** @brief Constructor
        @param outputStyle The output file format of the CSV file.
    */
    CSVFileWriter(OutputFormat outputStyle = OutputFormat::Default);
    ~CSVFileWriter();

    /** @brief Tell the CSVFileWriter to start accepting and writing data.
        @param path Full path to a CSV file to write out to.
        @return True, if able to open the file. Otherwise, false.
    */
    bool startWritingToFile(const char* path);

    /** @brief Prevents any other messages sent from being written. Flushes the CSV file and makes sure the file is safely closed. */
    void finalizeWriting();

    /** @brief Returns the path provided on construction. */
    const char* path() const;

    /** @brief Write out a Matrix4 camera pose at a timestamp to the CSV file.
        @param cameraPose The 4x4 pose to write out.
        @param timestampInUnitSpecifiedByFormat Seconds, if using TimestampInSeconds_PoseAsMatrix4, or milliseconds if using TimestampInMilliseconds_PoseAsVector6.
    */
    void writeCameraPose(const Matrix4& cameraPose, double timestampInUnitSpecifiedByFormat);
    
    /** @brief Write out a Vector3 at a timestamp to the CSV file.
        @param data The 3x1 vector to write out.
        @param timestampInUnitSpecifiedByFormat Seconds, if using TimestampInSeconds_PoseAsMatrix4, or milliseconds if using TimestampInMilliseconds_PoseAsVector6.
    */
    void writeVector3f(const Vector3f& data, double timestampInUnitSpecifiedByFormat);
    
    /** @brief Write out a Matrix4 as is without any conversion at a timestamp to the CSV file.
        @param data The 4x4 pose to write out.
        @param timestampInUnitSpecifiedByFormat Seconds, if using TimestampInSeconds_PoseAsMatrix4, or milliseconds if using TimestampInMilliseconds_PoseAsVector6.
    */
    void writeMatrix4(const Matrix4& data, double timestampInUnitSpecifiedByFormat);

    /** @brief Helper function to allow for non-RAII allocation. Sets the output format of the CSV. */
    void setOutputFormat(OutputFormat outputFormat);

    ST_DECLARE_OPAQUE_INTERNALS(CSVFileWriter);
};

} // ST namespace
