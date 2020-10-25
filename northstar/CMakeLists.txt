# cmake version guard
cmake_minimum_required(VERSION 3.10)

# target c++ 17
set(CMAKE_CXX_STANDARD 17)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

# sanity check vars
if(NOT DEFINED OPENVR_INCLUDE_DIR 
   OR NOT DEFINED OPENVR_LIB_DIR
   OR NOT DEFINED OPENVR_IMPLEMENTATION_DIR
   OR NOT DEFINED STRUCTURE_INCLUDE_DIR 
   OR NOT DEFINED STRUCTURE_LIB_DIR 
   OR NOT DEFINED STRUCTURE_IMPLEMENTATION_DIR
   OR NOT DEFINED LEAPSDK_INCLUDE_DIR
   OR NOT DEFINED LEAPSDK_LIB_DIR
   OR NOT DEFINED LEAPSDK_IMPLEMENTATION_DIR
   OR NOT DEFINED REALSENSE_INCLUDE_DIR
   OR NOT DEFINED REALSENSE_LIB_DIR
   OR NOT DEFINED REALSENSE_IMPLEMENTATION_DIR
   OR NOT DEFINED GLEW_INCLUDE_DIR
   OR NOT DEFINED EIGEN_INCLUDE_DIR
   OR NOT DEFINED TROMPELOEIL_INCLUDE_DIR)
    message(FATAL_ERROR "Please generate the project file using the CMakeLists file included in the root directory.")
endif()

# Driver DLL
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE TEST_SOURCES test/src/*.cpp)
file(GLOB_RECURSE TEST_HEADERS test/include/*.hpp)
add_library(northstar SHARED ${SOURCES};${HEADERS})
target_compile_features(northstar PRIVATE cxx_std_17)
target_compile_definitions(northstar PUBLIC -D_EXPORT)
target_include_directories(northstar PRIVATE include/)
target_include_directories(northstar PRIVATE ${OPENVR_INCLUDE_DIR})
target_link_libraries(northstar ${OPENVR_LIB_DIR}/openvr_api.lib)
target_include_directories(northstar PRIVATE ${STRUCTURE_INCLUDE_DIR})
target_link_libraries(northstar ${STRUCTURE_LIB_DIR}/Structure.lib)
target_include_directories(northstar PRIVATE ${LEAPSDK_INCLUDE_DIR})
target_link_libraries(northstar ${LEAPSDK_LIB_DIR}/x64/LeapC.lib)
target_include_directories(northstar PRIVATE ${REALSENSE_INCLUDE_DIR})
target_link_libraries(northstar ${REALSENSE_LIB_DIR}/realsense2.lib)
target_include_directories(northstar PRIVATE ${GLEW_INCLUDE_DIR})
target_include_directories(northstar PRIVATE ${EIGEN_INCLUDE_DIR})
target_include_directories(northstar PRIVATE ${DOCTEST_INCLUDE_DIR})
target_include_directories(northstar PRIVATE ${TROMPELOEIL_INCLUDE_DIR})
set_target_properties(northstar PROPERTIES PREFIX "driver_")

# Test suite
add_executable(northstar_tests ${TEST_SOURCES};${TEST_HEADERS})
target_compile_features(northstar_tests PRIVATE cxx_std_17)
target_include_directories(northstar_tests PRIVATE ${DOCTEST_INCLUDE_DIR})
target_include_directories(northstar_tests PRIVATE ${TROMPELOEIL_INCLUDE_DIR})
target_include_directories(northstar_tests PRIVATE include)
target_link_libraries(northstar_tests northstar)
target_include_directories(northstar_tests PRIVATE ${OPENVR_INCLUDE_DIR})
target_link_libraries(northstar_tests ${OPENVR_LIB_DIR}/openvr_api.lib)
target_include_directories(northstar PRIVATE ${LEAPSDK_INCLUDE_DIR})
target_link_libraries(northstar ${LEAPSDK_LIB_DIR}/x64/LeapC.lib)
target_include_directories(northstar_tests PRIVATE ${STRUCTURE_INCLUDE_DIR})
target_link_libraries(northstar_tests ${STRUCTURE_LIB_DIR}/Structure.lib)
target_include_directories(northstar PRIVATE ${REALSENSE_INCLUDE_DIR})
target_link_libraries(northstar ${REALSENSE_LIB_DIR}/realsense2.lib)

# copy dll for tests
add_custom_command(TARGET northstar_tests
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${STRUCTURE_IMPLEMENTATION_DIR}/Structure.dll
    $<TARGET_FILE_DIR:northstar_tests>
)

add_custom_command(TARGET northstar_tests
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${OPENVR_IMPLEMENTATION_DIR}/openvr_api.dll
    $<TARGET_FILE_DIR:northstar_tests>
)

add_custom_command(TARGET northstar_tests
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${LEAPSDK_IMPLEMENTATION_DIR}/x64/LeapC.dll
    $<TARGET_FILE_DIR:northstar_tests>
)

add_custom_command(TARGET northstar_tests
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${REALSENSE_IMPLEMENTATION_DIR}/realsense2.dll
    $<TARGET_FILE_DIR:northstar_tests>
)

# organize files better in visual studio
file(GLOB_RECURSE MATH_SOURCES src/math/*.cpp)
file(GLOB_RECURSE MATH_HEADERS include/math/*.hpp)
file(GLOB_RECURSE OPENVR_SOURCES src/openvr/*.cpp)
file(GLOB_RECURSE OPENVR_HEADERS include/openvr/*.hpp)
file(GLOB_RECURSE UTILITY_SOURCES src/utility/*.cpp)
file(GLOB_RECURSE UTILITY_HEADERS include/utility/*.hpp)
file(GLOB_RECURSE SPEC_SOURCES src/spec/*.cpp)
file(GLOB_RECURSE MOCK_HEADERS include/mock/*.hpp)
file(GLOB_RECURSE DRIVER_SOURCES src/driver/*.cpp)
file(GLOB_RECURSE DRIVER_HEADERS include/driver/*.hpp)
file(GLOB_RECURSE PLATFORM_SOURCES src/platform/*.cpp)

source_group("math" FILES ${MATH_SOURCES};${MATH_HEADERS})
source_group("openvr" FILES ${OPENVR_SOURCES};${OPENVR_HEADERS})
source_group("utility" FILES ${UTILITY_SOURCES};${UTILITY_HEADERS})
source_group("spec" FILES ${SPEC_SOURCES})
source_group("mock" FILES ${MOCK_HEADERS})
source_group("driver" FILES ${DRIVER_SOURCES};${DRIVER_HEADERS})
source_group("platform" FILES ${PLATFORM_SOURCES})

# set to test startup project
set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT "northstar_tests")

# installer stuff
install(TARGETS northstar
  DESTINATION northstar/bin/win64
  COMPONENT driver)

install(FILES
  ${STRUCTURE_IMPLEMENTATION_DIR}/Structure.dll
  ${OPENVR_IMPLEMENTATION_DIR}/openvr_api.dll
  ${LEAPSDK_IMPLEMENTATION_DIR}/x64/LeapC.dll
  ${REALSENSE_IMPLEMENTATION_DIR}/realsense2.dll
  DESTINATION northstar/bin/win64
  COMPONENT driver)

install(FILES
  ${CMAKE_SOURCE_DIR}/resources/northstar/driver.vrdrivermanifest
  DESTINATION northstar
  COMPONENT driver)

install(FILES
  ${CMAKE_SOURCE_DIR}/resources/northstar/resources/icons/humanhand_left.svg
  ${CMAKE_SOURCE_DIR}/resources/northstar/resources/icons/humanhand_right.svg
  DESTINATION northstar/resources/icons
  COMPONENT driver)

install(FILES
  ${CMAKE_SOURCE_DIR}/resources/northstar/resources/input/northstar_legacy_bindings.json
  ${CMAKE_SOURCE_DIR}/resources/northstar/resources/input/northstar_profile.json
  ${CMAKE_SOURCE_DIR}/resources/northstar/resources/input/steamvr_compositor_bindings_northstar.json
  ${CMAKE_SOURCE_DIR}/resources/northstar/resources/input/steamvr_home_bindings_northstar.json
  ${CMAKE_SOURCE_DIR}/resources/northstar/resources/input/steamvr_lab_bindings_northstar.json
  DESTINATION northstar/resources/input
  COMPONENT driver)

install(FILES
  ${CMAKE_SOURCE_DIR}/resources/northstar/resources/settings/default.vrsettings
  DESTINATION northstar/resources/settings
  COMPONENT driver)

install(FILES
  ${CMAKE_SOURCE_DIR}/CREDITS.md
  ${CMAKE_SOURCE_DIR}/README.md
  ${CMAKE_SOURCE_DIR}/TODO.md
  ${CMAKE_SOURCE_DIR}/LICENSE
  DESTINATION .
  COMPONENT driver)

include(InstallRequiredSystemLibraries)
install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
  DESTINATION northstar/bin/win64
	COMPONENT driver)

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME "Northstar SteamVR Driver")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SteamVR Driver for the project northstar AR headset")
set(CPACK_PACKAGE_VERSION "0.1.2")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Northstar SteamVR Driver")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Northstar SteamVR Driver")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/resources/installer/POST_INSTALL.md")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")
	
include(CPack)

cpack_add_component(northstar
	DISPLAY_NAME "Northstar SteamVR Driver"
	DESCRIPTION "Northstar SteamVR Driver")
