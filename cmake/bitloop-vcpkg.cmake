# cmake/bitloop-vcpkg.cmake
if(DEFINED ENV{BITLOOP_ROOT})
  set(_vcpkg_file "$ENV{BITLOOP_ROOT}/vcpkg/scripts/buildsystems/vcpkg.cmake")
elseif(DEFINED ENV{VCPKG_ROOT})
  set(_vcpkg_file "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
  message(FATAL_ERROR
    "Neither BITLOOP_ROOT nor VCPKG_ROOT set. "
    "Please run bitloop bootstrap or set VCPKG_ROOT.")
endif()

if(NOT EXISTS "${_vcpkg_file}")
  message(FATAL_ERROR
    "vcpkg toolchain not found at ${_vcpkg_file}")
endif()

include("${_vcpkg_file}")
