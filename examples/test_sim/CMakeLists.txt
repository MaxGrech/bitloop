cmake_minimum_required(VERSION 3.21)
project(test_sim LANGUAGES CXX)

# Link to bitloop (unless monorepo project)
if (NOT TARGET bitloop::bitloop)
  find_package(bitloop CONFIG REQUIRED)
endif()

file(GLOB SIM_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.h")

# 1. Setup project dependencies (if any)
#bitloop_add_dependency(../test_sim)

# 2. Create new project
bitloop_new_project(test_sim ${SIM_SOURCES})

