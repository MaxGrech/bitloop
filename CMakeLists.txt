cmake_minimum_required(VERSION 3.21)

project(bitloop-superbuild NONE)

add_subdirectory(framework)

#if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#  if (DEFINED ENV{VCPKG_ROOT})
#    set(CMAKE_TOOLCHAIN_FILE
#      "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
#      CACHE FILEPATH "vcpkg toolchain file" FORCE
#    )
#  else()
#    message(FATAL_ERROR
#      "You must set VCPKG_ROOT or pass -DCMAKE_TOOLCHAIN_FILE to point at vcpkg."
#    )
#  endif()
#endif()
