cmake_minimum_required(VERSION 3.21)
project(bitloop VERSION 0.9 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

# ────────── Bootstrap package helpers for both in‑tree & installed modes ──────────
include(bitloopSimulation)


# ──────────────────────────────────────────────────────────────────
# ──────────────────────  Framework Sources  ───────────────────────
# ──────────────────────────────────────────────────────────────────

set(BL_INC "${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop")
set(BL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB CORE_SOURCES CONFIGURE_DEPENDS
    "${BL_INC}/bitloop.h"
    "${BL_SRC}/bitloop.cpp"
    "${BL_INC}/core/*.h"
    "${BL_SRC}/core/*.cpp"
)

file(GLOB UTILITY_SOURCES CONFIGURE_DEPENDS
    "${BL_SRC}/utility/*.cpp"
    "${BL_INC}/utility/*.h"
)

file(GLOB UI_SOURCES CONFIGURE_DEPENDS
    "${BL_SRC}/ui/*.cpp"
    "${BL_INC}/ui/*.h"
)

file(GLOB GRAPHICS_SOURCES CONFIGURE_DEPENDS
    "${BL_SRC}/graphics/*.cpp"
    "${BL_INC}/graphics/*.h"
)

file(GLOB EXTERNAL_HEADERS CONFIGURE_DEPENDS
    "${BL_INC}/3rd_party/*.hpp"
    "${BL_INC}/3rd_party/*.h"
    "${BL_INC}/3rd_party/nanosvg/*.h"
    "${BL_INC}/3rd_party/glad/*.h"
    "${BL_INC}/3rd_party/glad/KHR/*.h"
    "${BL_INC}/3rd_party/nanovg/*.h"
    "${BL_INC}/3rd_party/concurrentqueue/*.h"
)

file(GLOB EXTERNAL_SOURCES CONFIGURE_DEPENDS
    "${BL_SRC}/3rd_party/*.cpp"
    "${BL_SRC}/3rd_party/*.c"
    "${BL_SRC}/3rd_party/glad/*.c"
    "${BL_SRC}/3rd_party/nanovg/*.c"
)

# ──────── Mark all .c files as c++ ────────
foreach(_cfile IN LISTS EXTERNAL_SOURCES)
  if(_cfile MATCHES "\\.c$")
    set_source_files_properties(${_cfile} PROPERTIES LANGUAGE CXX)
  endif()
endforeach()


set(ALL_SRC_FILES
    ${CORE_SOURCES}
    ${UTILITY_SOURCES}
    ${UI_SOURCES}
    ${GRAPHICS_SOURCES}
    ${EXTERNAL_HEADERS}
    ${EXTERNAL_SOURCES}
)


add_library(bitloop STATIC ${ALL_SRC_FILES})

target_compile_features(bitloop PUBLIC cxx_std_23)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    # Compilation flags
    target_compile_options(bitloop PRIVATE
        "-O3"
        "-sUSE_PTHREADS=1"
        "-pthread"
        "-matomics"
        "-mbulk-memory"
    )

    target_link_options(bitloop PRIVATE
        "-sEXPORTED_FUNCTIONS=[]"
        "-sUSE_SDL=3"
        "-sUSE_WEBGL2=1"
        "-sFULL_ES3=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sUSE_PTHREADS=1"
        "-sPTHREAD_POOL_SIZE=16"
    )
endif()

target_include_directories(bitloop
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/ui>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/core>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/utility>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/graphics>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/3rd_party>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/3rd_party/nanovg>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop/3rd_party/glad>

    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:include/bitloop>
    $<INSTALL_INTERFACE:include/bitloop/core>
    $<INSTALL_INTERFACE:include/bitloop/utility>
    $<INSTALL_INTERFACE:include/bitloop/ui>
    $<INSTALL_INTERFACE:include/bitloop/graphics>
    $<INSTALL_INTERFACE:include/bitloop/3rd_party>
    $<INSTALL_INTERFACE:include/bitloop/3rd_party/nanovg>
    $<INSTALL_INTERFACE:include/bitloop/3rd_party/glad>

  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utility
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics
)

# ──────────────────────────────────────────────────────────────────
# ─────────────────────────  Dependencies  ─────────────────────────
# ──────────────────────────────────────────────────────────────────

find_package(imgui CONFIG REQUIRED)
find_package(Freetype CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(bitloop
  PUBLIC
    Freetype::Freetype
    imgui::imgui
    SDL3::SDL3
    glm::glm
)

add_library(bitloop::bitloop ALIAS bitloop)

# ──────── Install static libs ────────
install(TARGETS bitloop
    EXPORT bitloopTargets
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin
)

# ──────── Install public headers ────────
install(DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/include/bitloop"
    DESTINATION include
    FILES_MATCHING
      PATTERN "*.h"
      PATTERN "*.hpp"
)

# ──────── Export CMake config file for find_package() ────────
install(EXPORT bitloopTargets
    NAMESPACE bitloop::
    DESTINATION share/bitloop
)

# ──────── Install framework main entry‐point template ────────
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bitloop_main.cpp"
    DESTINATION share/bitloop/src
    RENAME bitloop_main.cpp
)

# ──────── Install default emscripten shell.html ────────
#install(FILES
#    "${CMAKE_CURRENT_SOURCE_DIR}/common/static/shell.html"
#    DESTINATION share/bitloop/static
#    RENAME shell.html
#)

# ──────── Install common ────────
install(DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/common"
    DESTINATION share/bitloop/common
)

# ──────── Install simulation cmake helpers ────────
install(FILES
   "${CMAKE_CURRENT_SOURCE_DIR}/bitloopSimulation.cmake"
   DESTINATION share/bitloop
 )

# ──────── Config & Version file ────────
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/bitloopConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/bitloopConfig.cmake"
    INSTALL_DESTINATION share/bitloop)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/bitloopConfigVersion.cmake"
    VERSION  ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/bitloopConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/bitloopConfigVersion.cmake"
    DESTINATION share/bitloop)