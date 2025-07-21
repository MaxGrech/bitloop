#set(BL_SRC_DIR "" CACHE PATH "framework source dir")
#set(BL_BIN_DIR "" CACHE PATH "framework build dir")
#message(STATUS "BL_SRC_DIR = ${BL_SRC_DIR}")
#message(STATUS "BL_BIN_DIR = ${BL_BIN_DIR}")

#add_subdirectory("${BL_SRC_DIR}" "${BL_BIN_DIR}" EXCLUDE_FROM_ALL)
#message(STATUS "Framework’s CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")


#message(STATUS "VCPKG_OVERLAY_PORTS = $ENV{VCPKG_OVERLAY_PORTS}")

cmake_minimum_required(VERSION 3.21)
project(test_sim LANGUAGES CXX)

file(GLOB SIM_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(test_sim ${SIM_SOURCES})

find_package(bitloop CONFIG REQUIRED)
target_link_libraries(test_sim PRIVATE bitloop::bitloop)


