cmake_minimum_required(VERSION 3.16)
project(bitloop VERSION 0.9 LANGUAGES CXX)

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

message(STATUS "Using CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

include(FetchContent)
message(STATUS "Using FETCHCONTENT_BASE_DIR = ${FETCHCONTENT_BASE_DIR}")

# ───────────────────  SDL3  ───────────────────

if (NOT EMSCRIPTEN)
  message(STATUS "Fetching and building SDL3")

  set(SDL_TEST          OFF CACHE BOOL   "" FORCE)
  set(SDL_SHARED        OFF CACHE BOOL   "" FORCE)
  set(SDL_STATIC        ON  CACHE BOOL   "" FORCE)
  set(SDL_DEBUG_POSTFIX ""  CACHE STRING "" FORCE)

  FetchContent_Declare(SDL3 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-3.2.16)
  FetchContent_MakeAvailable(SDL3)

  add_library(SDL3::SDL3-static ALIAS SDL3-static)   # alias so we can link against SDL3::SDL3-static

  # ensure bitloop sees SDL3 headers & library
  target_link_libraries(bitloop PUBLIC SDL3::SDL3-static)
  target_include_directories(bitloop PUBLIC ${SDL3_SOURCE_DIR}/include)

else()
  message(STATUS "Configuring bitloop SDL3 for Emscripten")

  target_compile_options(bitloop PRIVATE "-sUSE_SDL=3" "-sUSE_PTHREADS=1" "-pthread" "-matomics" "-mbulk-memory")
  target_link_options(bitloop    PRIVATE "-sUSE_SDL=3" "-sUSE_PTHREADS=1" "-sALLOW_MEMORY_GROWTH=1")

  # ensure bitloop sees sysroot headers
  target_include_directories(bitloop PUBLIC "$ENV{EMSDK}/upstream/emscripten/cache/sysroot/include")
  
  # Add single-line alias header so <SDL3/SDL.h> resolves
  ## set(SDL3_ALIAS_DIR "${CMAKE_BINARY_DIR}/sdl3_alias/include/SDL3")
  ## file(MAKE_DIRECTORY "${SDL3_ALIAS_DIR}")
  ## file(WRITE "${SDL3_ALIAS_DIR}/SDL.h" "#pragma once\n#include <SDL/SDL.h>\n")
  ## target_include_directories(${target} PRIVATE "${CMAKE_BINARY_DIR}/sdl3_alias/include")
endif()

add_library(bitloop::bitloop ALIAS bitloop)