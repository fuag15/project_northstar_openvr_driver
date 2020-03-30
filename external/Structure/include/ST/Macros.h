/*
    Macros.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Structure SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

/** @brief Macros.h holds different preprocessor macros for use in the system.
    @brief Please to not modify any of the defines below.
*/

#pragma once

#if _WIN32
#   if STRUCTURE_EXPORTS
#       define ST_API __declspec(dllexport)
#   elif LINKING_AGAINST_STRUCTURE_STATIC
#       define ST_API // nothing, dllimport would make it fail to find the symbols from the static lib.
#   else
#       define ST_API __declspec(dllimport)
#   endif
#    define ST_PRIVATE_API // FIXME: Find a way to override class-wide visibility specifiers on windows, for specific methods.
#    define ST_CDECL __cdecl
#else
#    define ST_API         __attribute__((visibility("default")))
#    define ST_PRIVATE_API __attribute__((visibility("hidden")))
#    define ST_CDECL
#endif

#if __cplusplus
#   define ST_PLAIN_C_BEGIN extern "C" {
#   define ST_PLAIN_C_END }
#else
#   define ST_PLAIN_C_BEGIN
#   define ST_PLAIN_C_END
#endif

#if _WIN32
#    define ST_ALIGNED(Alignment) __declspec(align(Alignment))
#else
#    define ST_ALIGNED(Alignment) __attribute__((aligned(Alignment)))
#endif

#define ST_NON_COPYABLE(Class)                  \
    Class (const Class&) = delete;              \
    Class& operator = (const Class&) = delete

#define ST_DEFAULT_CONSTRUCTIBLE(Class) \
    Class () = default

#define ST_DEFAULT_COPYABLE(Class)               \
    Class (const Class&) = default;              \
    Class& operator = (const Class&) = default

#define ST_DECLARE_THAT(Class)                          \
public:                                                 \
    struct That;                                        \
    ST_PRIVATE_API Class (That* that_) : that(that_) {} \
    That* that

#define ST_DECLARE_OPAQUE_INTERNALS(Class) \
    ST_NON_COPYABLE(Class);                \
    ST_DECLARE_THAT(Class)

#define ST_DECLARE_CLONABLE_OPAQUE_INTERNALS(Class) \
    Class (const Class& copy);                      \
    Class& operator= (const Class& rhs);          \
    ST_DECLARE_THAT(Class)

#if _MSC_VER && !defined(__clang__)
#   define ST_WEAK
#else
#   define ST_WEAK __attribute__((weak))
#endif
