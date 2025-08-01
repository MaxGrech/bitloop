# Resolve VCPKG_ROOT (adjust to your layout)
#if(NOT DEFINED VCPKG_ROOT)
#  set(VCPKG_ROOT "C:/dev/cpp/libs/vcpkg")  # or ${CMAKE_SOURCE_DIR}/extern/vcpkg
#endif()

message(STATUS "~~~~~~~~~~~~~~~~~~~ Using bitloop toolchain wrapper ~~~~~~~~~~~~~~~~~~~")

# Choose the ABI based on OS
if(WIN32)
  set(_ABI "x64-windows")
elseif(APPLE)
  set(_ABI "x64-osx")
elseif(UNIX)
  set(_ABI "x64-linux")
endif()

if(VCPKG_BUILD_STATIC)
  #message(STATUS "@@@ Making static")
  set(_ABI "${_ABI}-static")
endif()

#message(STATUS "_ABI: ${_ABI}")

# Force the triplet for vcpkg
set(VCPKG_TARGET_TRIPLET "${_ABI}" CACHE STRING "" FORCE)
message(STATUS "@@@ USING TRIPLET: ${VCPKG_TARGET_TRIPLET} @@@")


#message(STATUS "_ABI: ${_ABI}")
#message(STATUS "TESTV: ${TESTV}")
#message(STATUS "VCPKG_BUILD_STATIC: ${VCPKG_BUILD_STATIC}")
#message(STATUS "VCPKG_ROOT: $ENV{VCPKG_ROOT}")
#message(STATUS "VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

# Let host triplet default unless cross-compiling tools matter to you
include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
