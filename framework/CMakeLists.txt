cmake_minimum_required(VERSION 3.21)
project(bitloop VERSION 0.9 LANGUAGES CXX)

message(STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")
message(STATUS "PROJECT_NAME = ${PROJECT_NAME}")


# ──────────────────────────────────────────────────────────────────
# ──────────────────────  Framework Sources  ───────────────────────
# ──────────────────────────────────────────────────────────────────

file(GLOB CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/core/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp"
)

add_library(bitloop STATIC ${CORE_SOURCES})

target_include_directories(bitloop
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>

  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    
    ${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/core

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/3rd_party>
)


# ──────────────────────────────────────────────────────────────────
# ─────────────────────────  Dependencies  ─────────────────────────
# ──────────────────────────────────────────────────────────────────

#message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
#message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_SOURCE_DIR}")

#message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
#message(STATUS "CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")

#include(FetchContent)

# ───────────────────  SDL3  ───────────────────
#if (NOT TARGET SDL3::SDL3-static)
#	if (NOT EMSCRIPTEN)
#		message(STATUS "Fetching and building SDL3")
#
#		set(SDL_TEST          OFF CACHE BOOL   "" FORCE)
#		set(SDL_SHARED        OFF CACHE BOOL   "" FORCE)
#		set(SDL_STATIC        ON  CACHE BOOL   "" FORCE)
#		set(SDL_DEBUG_POSTFIX ""  CACHE STRING "" FORCE)
#
#		FetchContent_Declare(SDL3 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-3.2.16)
#		FetchContent_MakeAvailable(SDL3)
#
#		add_library(SDL3::SDL3-static ALIAS SDL3-static)   # alias so we can link against SDL3::SDL3-static
#
#		# ensure bitloop sees SDL3 headers & library
#		target_link_libraries(bitloop PUBLIC SDL3::SDL3-static)
#		target_include_directories(bitloop PUBLIC ${SDL3_SOURCE_DIR}/include)
#
#	else()
#		message(STATUS "Configuring bitloop SDL3 for Emscripten")
#
#		target_compile_options(bitloop PRIVATE "-sUSE_SDL=3" "-sUSE_PTHREADS=1" "-pthread" "-matomics" "-mbulk-memory")
#		target_link_options(bitloop    PRIVATE "-sUSE_SDL=3" "-sUSE_PTHREADS=1" "-sALLOW_MEMORY_GROWTH=1")
#
#		# ensure bitloop sees sysroot headers
#		target_include_directories(bitloop PUBLIC "$ENV{EMSDK}/upstream/emscripten/cache/sysroot/include")
#
#		# Add single-line alias header so <SDL3/SDL.h> resolves
#		## set(SDL3_ALIAS_DIR "${CMAKE_BINARY_DIR}/sdl3_alias/include/SDL3")
#		## file(MAKE_DIRECTORY "${SDL3_ALIAS_DIR}")
#		## file(WRITE "${SDL3_ALIAS_DIR}/SDL.h" "#pragma once\n#include <SDL/SDL.h>\n")
#		## target_include_directories(${target} PRIVATE "${CMAKE_BINARY_DIR}/sdl3_alias/include")
#	endif()
#else()
#	  message(STATUS "SDL3 already built")
#endif()


find_package(imgui CONFIG REQUIRED)    # will find the freetype‐enabled build
find_package(Freetype CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# List all imported targets and print the ones that look like FreeType
#get_property(_allTargets GLOBAL PROPERTY TARGETS)
#foreach(_t IN LISTS _allTargets)
#  if(_t MATCHES "(?i)freetype")
#     message(STATUS "🔍 FreeType candidate target: ${_t}")
#  endif()
#endforeach()

target_link_libraries(bitloop
  PUBLIC
    Freetype::Freetype
    imgui::imgui
    SDL3::SDL3-shared
    glm::glm
)

add_library(bitloop::bitloop ALIAS bitloop)


# 1) Install the static lib (or DLL) into lib/
install(TARGETS bitloop
    EXPORT bitloopTargets
    ARCHIVE  DESTINATION lib            # .lib or .a
    LIBRARY  DESTINATION lib            # .dll on Windows, .so on *nix
    RUNTIME  DESTINATION bin            # .exe on Windows, if any
)

# 2) Install public headers
install(DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop"
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# 3) Export a CMake config file for find_package()
install(EXPORT bitloopTargets
    #FILE bitloopConfig.cmake # Use your cmake.in instead of the auto-generated one
    NAMESPACE bitloop::
    DESTINATION share/bitloop
)


# ─── CONFIG FILE & VERSION FILE ──────────────────
include(CMakePackageConfigHelpers)

message(STATUS "[bitloop] About to configure bitloopConfig.cmake from .in")
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/bitloopConfig.cmake.in"        # your template
    "${CMAKE_CURRENT_BINARY_DIR}/bitloopConfig.cmake"           # output path
    INSTALL_DESTINATION share/bitloop)                          # same dir as targets

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/bitloopConfigVersion.cmake"
    VERSION  ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/bitloopConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/bitloopConfigVersion.cmake"
    DESTINATION share/bitloop)
# ──────────────────────────────────────────────────────────