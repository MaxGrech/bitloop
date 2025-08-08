# bitloop/CMakeLists.txt

cmake_minimum_required(VERSION 3.21)

message(STATUS "Path:")
message(STATUS "$ENV{PATH}")

project(bitloop-superbuild LANGUAGES CXX)

add_subdirectory(framework)
add_subdirectory(examples)