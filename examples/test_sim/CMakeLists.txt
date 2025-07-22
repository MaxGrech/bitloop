cmake_minimum_required(VERSION 3.21)
project(test_sim LANGUAGES CXX)

file(GLOB SIM_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(test_sim ${SIM_SOURCES})

if(NOT TARGET bitloop::bitloop)
	find_package(bitloop CONFIG REQUIRED)
endif()

target_link_libraries(test_sim PRIVATE bitloop::bitloop)


