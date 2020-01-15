cmake_minimum_required(VERSION 3.5)
project(Northstar_OpenVR CXX)

set(OPENVR_LIB_DIR ${CMAKE_SOURCE_DIR}/external/openvr/lib/win64 CACHE STRING "OpenVR .lib directory")
set(OPENVR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/openvr/headers CACHE STRING "OpenVR include directory")
set(OPENVR_IMPLEMENTATION_DIR ${CMAKE_SOURCE_DIR}/external/openvr/bin/win64 CACHE STRING "OpenVR implementation (dll, so, dylib)")
set(STRUCTURE_LIB_DIR ${CMAKE_SOURCE_DIR}/external/structure/lib CACHE STRING "Perception Engine .lib directory")
set(STRUCTURE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/structure/include CACHE STRING "Perception Engine include directory")
set(STRUCTURE_IMPLEMENTATION_DIR ${CMAKE_SOURCE_DIR}/external/structure/bin CACHE STRING "Perception Engine implementation (dll, so, dylib)")
set(GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glew/include CACHE STRING "Glew include directory")
set(EIGEN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/eigen CACHE STRING "Eigen include directory")
set(DOCTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/doctest CACHE STRING "Doctest include directory")
set(TROMPELOEIL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/trompeloeil/include CACHE STRING "Trompeloeil include directory")
set(LEAPSDK_LIB_DIR ${CMAKE_SOURCE_DIR}/external/LeapSDK/lib CACHE STRING "Leap SDK lib directory")
set(LEAPSDK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/LeapSDK/include CACHE STRING "Leap SDK include directory")
set(LEAPSDK_IMPLEMENTATION_DIR ${CMAKE_SOURCE_DIR}/external/LeapSDK/lib CACHE STRING "Leap SDK implementation directory")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/D_UNICODE>")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/DUNICODE>")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/D_UNICODE>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/DUNICODE>")

add_subdirectory(northstar)