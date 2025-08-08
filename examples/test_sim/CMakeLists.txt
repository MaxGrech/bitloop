cmake_minimum_required(VERSION 3.21)
project(test_sim  LANGUAGES CXX)

# Link to bitloop
if (NOT TARGET bitloop::bitloop)
  find_package(bitloop CONFIG REQUIRED)
endif()

file(GLOB SIM_SOURCES CONFIGURE_DEPENDS "test_sim/*.cpp" "test_sim/*.h")

# 2. Create new project
bitloop_new_project(test_sim  ${SIM_SOURCES})

