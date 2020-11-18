cmake_minimum_required(VERSION 3.14)
project(NorthStarDriver
  LANGUAGES NONE

set(ZIPFN ${CMAKE_CURRENT_BINARY_DIR}/LeapDeveloperKit_4.0.0+52238_win.zip)
set(OUTFN ${CMAKE_CURRENT_BINARY_DIR}/LeapDeveloperKit_4.0.0+52238_win.zip)

if(NOT EXISTS ${OUTFN})
  if(NOT EXISTS ${ZIPFN})
    file(DOWNLOAD https://github.com/leapmotion/UnityModules/raw/feat-multi-device/Multidevice%20Service/LeapDeveloperKit_4.0.0%2B52238_win.zip ${ZIPFN}
         SHOW_PROGRESS
         EXPECTED_HASH MD5=6F14B5D7CC447F05F8878FB8DC9545BE)
  endif(NOT EXISTS ${ZIPFN})

  # this is OS-agnostic
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${ZIPFN}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif(NOT EXISTS ${OUTFN})

## -- check extracted (or any other) file
#file(MD5 ${OUTFN} CHKSUM)

#if(NOT ${CHKSUM} STREQUAL 6F14B5D7CC447F05F8878FB8DC9545BE)
#  message(WARNING "MD5 checksum did not match ${OUTFN}")
#endif()