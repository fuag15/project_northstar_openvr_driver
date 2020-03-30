/*
    This file is part of the Bridge Engine SDK.
    Copyright © 2019 Occipital, Inc. All rights reserved.
    http://structure.io
*/

#pragma once

#include <ST/MathTypes.h>
#include <ST/Serialization.h>
#include <cstdint>

namespace ST {

//------------------------------------------------------------------------------

class ST_API SharedMemoryBuffer
{
public:
    enum InitializationMode
    {
        Create, // Fails when the buffer already exist.
        Open,   // Fails when the buffer does not exist.
        Auto,   // Never fails and either creates or opens the buffer.
    };
    
    enum AccessType
    {
        ReadWrite, // Use for process output.
        ReadOnly,  // Use for process input.
    };
    
    enum SynchronizationMode
    {
        Current, // Commits buffers without signaling writes and acquires them without waiting for updates. Use this for global state publishing and polling.
        Next,    // Signals writes when commiting buffers and blocks until they are updated while acquiring them. Use this to implement half-duplex conversations.
    };

    static constexpr size_t InvalidSize = size_t(-1);

    static const unsigned long BlockForever;

public:
     SharedMemoryBuffer (InitializationMode initMode, AccessType accessType, SynchronizationMode syncMode, const char* identifier, size_t size);
    ~SharedMemoryBuffer ();

public:
    size_t size () const;
    bool isInitialized () const;

public:
    bool initialize (); // Call this once successfully before calling anything below.
    bool release ();

public:
    bool  readByte (uint8_t& byte, unsigned long timeoutMsecs = BlockForever);
    bool writeByte (uint8_t  byte, unsigned long timeoutMsecs = BlockForever);

    bool  readInteger (int& val, unsigned long timeoutMsecs = BlockForever);
    bool writeInteger (int  val, unsigned long timeoutMsecs = BlockForever);

    size_t  readData (      uint8_t* buffer, size_t size, unsigned long timeoutMsecs = BlockForever);
    size_t writeData (const uint8_t* buffer, size_t size, unsigned long timeoutMsecs = BlockForever);

public:
    uint8_t* acquireOutputBuffer (size_t   sizeInBytes , unsigned long timeoutMsecs = BlockForever);
    void     releaseOutputBuffer (uint8_t* outputBuffer);

public:
    const uint8_t* acquireInputBuffer (unsigned long timeoutMsecs = BlockForever); // FIXME: Report failures.
    void           releaseInputBuffer (const uint8_t* inputBuffer); // FIXME: Report failures.

    ST_DECLARE_OPAQUE_INTERNALS(SharedMemoryBuffer);
};

//------------------------------------------------------------------------------

struct ST_API SystemWideActivity
{
     SystemWideActivity (const char* name);
    ~SystemWideActivity ();

    bool start ();
    void stop ();

    bool isRunningInThisProcess () const;
    bool isRunningInAnotherProcess () const;
    bool isRunningInAnyProcess () const;

    ST_DECLARE_OPAQUE_INTERNALS(SystemWideActivity);
};

//------------------------------------------------------------------------------

struct ST_API SystemWideMutex
{
    SystemWideMutex(const char* name);
    ~SystemWideMutex();

    void lock();
    void unlock();

    ST_DECLARE_OPAQUE_INTERNALS(SystemWideMutex);
};

struct ST_API SystemWideMutexScopedLock
{
    SystemWideMutexScopedLock(SystemWideMutex* mutex);
    ~SystemWideMutexScopedLock();

    ST_DECLARE_OPAQUE_INTERNALS(SystemWideMutexScopedLock);
};

//------------------------------------------------------------------------------

} // ST namespace
