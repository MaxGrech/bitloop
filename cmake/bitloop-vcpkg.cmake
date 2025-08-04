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


# If the user chose Ninja as the generator, or if no CMAKE_MAKE_PROGRAM is set,
# attempt to find a vendored vcpkg ninja or fall back to system ninja.
if(CMAKE_GENERATOR STREQUAL "Ninja" OR NOT DEFINED CMAKE_MAKE_PROGRAM)
  if(WIN32)
    set(_ninja_path "${_vcpkg_root}/installed/x64-windows/tools/ninja/ninja.exe")
  else()
    set(_ninja_path "${_vcpkg_root}/installed/${VCPKG_TARGET_TRIPLET}/tools/ninja/ninja")
  endif()

  if(EXISTS "${_ninja_path}")
    message(STATUS "Using vendored Ninja at ${_ninja_path}")
    set(CMAKE_MAKE_PROGRAM "${_ninja_path}" CACHE FILEPATH "Path to Ninja" FORCE)
  else()
    message(STATUS "Falling back to system Ninja")
    find_program(CMAKE_MAKE_PROGRAM ninja)
    if(NOT CMAKE_MAKE_PROGRAM)
      message(FATAL_ERROR "Ninja not found on PATH; please install it or add it to vcpkg.json")
    endif()
  endif()
endif()


# Include vcpkg toolchain
include("${_vcpkg_file}")
