cmake_minimum_required(VERSION 3.16)
project(test_sim LANGUAGES CXX)

set(BL_SRC_DIR "" CACHE PATH "framework source dir")
set(BL_BIN_DIR "" CACHE PATH "framework build dir")

include(FetchContent)
FetchContent_Declare(bitloop SOURCE_DIR "${BL_SRC_DIR}" BINARY_DIR "${BL_BIN_DIR}")
FetchContent_MakeAvailable(bitloop)

file(GLOB SIM_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(test_sim ${SIM_SOURCES})
target_link_libraries(test_sim PRIVATE bitloop::bitloop)
