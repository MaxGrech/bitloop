cmake_minimum_required(VERSION 3.21)
project(test_sim LANGUAGES CXX)

# Link to bitloop (unless monorepo project)
if (NOT TARGET bitloop::bitloop)
  find_package(bitloop CONFIG REQUIRED)
endif()


file(GLOB SIM_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.h")

# Setup simulation with local source files
bitloop_add_simulation(test_sim ${SIM_SOURCES})

# Add simulation dependencies
#bitloop_add_dependency(../test_sim)
