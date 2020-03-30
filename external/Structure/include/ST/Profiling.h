/*
    Profiling.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Structure SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

#pragma once

#include <ST/Macros.h>
#include <ST/Utilities.h>

#include <algorithm>
#include <functional>
#include <limits>
#include <string>

namespace ST
{
    /** @brief Path of the output folder for profiling logs. */
    ST_API const char* createdProfilingFolder();

    struct OnlineMeanEstimator
    {
        double n = 0.;
        double mean = 0.;
        double minValue = std::numeric_limits<double>::max ();  // makes sure it will get
                                                                // overwritten by the
                                                                // first value
        double maxValue = std::numeric_limits<double>::lowest ();  // make sure it will
                                                                   // get overwritten by
                                                                   // the first value

        // From Knuth
        // http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm.
        void update (double x)
        {
            mean = (mean * n + x) / (n + 1.0);
            n += 1.0;

            minValue = std::min (minValue, x);
            maxValue = std::max (maxValue, x);
        }

        void update (const OnlineMeanEstimator& rhs)
        {
            if (rhs.n < 0.5) return;
            mean = (mean * n + rhs.mean * rhs.n) / (n + rhs.n);
            n += rhs.n;

            minValue = std::min (rhs.minValue, minValue);
            maxValue = std::max (rhs.maxValue, maxValue);
        }
    };

    //------------------------------------------------------------------------------

    // Utility class to monitor the performance of a real-time system.
    class ST_API PerformanceMonitor
    {
    public:
        PerformanceMonitor ();
        ~PerformanceMonitor ();

    public:
        double averageFpsFromFrameCount () const;
        double samplingPeriodInSeconds () const;
        int numSamples () const;

        double meanValue () const;
        double minValue () const;
        double maxValue () const;

        double meanPeriod () const;
        double minPeriod () const;
        double maxPeriod () const;

        double timestampOfFirstSample() const;
        double valueOfLastSample() const;

    public:
        void addSampleWithTimestamp (const double sampleTimestamp,
                                     const double sampleValue);

        void addSample (double sampleValue)
        {
            addSampleWithTimestamp (getTimestampNow (), sampleValue);
        }

        void startNewSequenceFromLastSample ();

    private:
        ST_DECLARE_OPAQUE_INTERNALS (PerformanceMonitor);
    };

    //------------------------------------------------------------------------------

    class ScopeTimeForPerformanceMonitor
    {
    public:
        ScopeTimeForPerformanceMonitor (
            PerformanceMonitor& monitor,
            double samplingPeriodBeforeCallbackAndRestart = 1.0,
            std::function<void(const PerformanceMonitor& monitor)> callBackAfterPeriod =
                nullptr)
            : _monitor (monitor), _startTime (getTimestampNow ()),
              _samplingPeriodBeforeCallbackAndRestart (
                  samplingPeriodBeforeCallbackAndRestart),
              _callBackAfterPeriod (callBackAfterPeriod)
        {
        }

        ~ScopeTimeForPerformanceMonitor ()
        {
            _monitor.addSample (getTimestampNow () - _startTime);

            if (_monitor.samplingPeriodInSeconds () >
                _samplingPeriodBeforeCallbackAndRestart)
            {
                if (_callBackAfterPeriod) _callBackAfterPeriod (_monitor);
                _monitor.startNewSequenceFromLastSample ();
            }
        }

    private:
        PerformanceMonitor& _monitor;
        double _startTime = NAN;
        double _samplingPeriodBeforeCallbackAndRestart = NAN;
        std::function<void(const PerformanceMonitor& monitor)> _callBackAfterPeriod = nullptr;
    };

    //------------------------------------------------------------------------------

    // Use the macros below instead
    class ST_API TimeCountDoNotUseDirectly
    {
    public:
        TimeCountDoNotUseDirectly (const std::string& name, int debug_level = 1,
                                   double minDurationInSecondToShow = -1.0 /* no min */);
        ~TimeCountDoNotUseDirectly ();

        double elapsedMsecs (const std::string& marker = "") const;

        double elapsedMsecsNoPrint () const;

        double stop (const std::string& marker = "");

    private:
        ST_DECLARE_OPAQUE_INTERNALS(TimeCountDoNotUseDirectly);
    };

#define ST_TIMECOUNT(VarName, Name, DebugLevel)                                          \
    ST::TimeCountDoNotUseDirectly VarName (Name, DebugLevel)
#define ST_TIMECOUNT_IF_HIGHER_THAN(VarName, Name, DebugLevel, MinDurationSeconds)       \
    ST::TimeCountDoNotUseDirectly VarName (Name, DebugLevel, MinDurationSeconds)
#define ST_TIMECOUNT_ELAPSED(VarName, Marker) VarName.elapsedMsecs (Marker)
#define ST_TIMECOUNT_STOP(VarName) VarName.stop ()

    /** 
        The profiling file will go in the same folder as all the Occipital profiler output, 
        usually under AppDocuments/Occipital/profiling .
    */
    struct ST_API ProfilingFileLogger
    {
        ProfilingFileLogger(const std::string& fileName, const char* firstLine);
        ~ProfilingFileLogger();
        void printf(const char * format, ...);

    private:
        ST_DECLARE_OPAQUE_INTERNALS(ProfilingFileLogger);
    };

} // ST namespace
