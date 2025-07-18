cmake_minimum_required(VERSION 3.16)
project(bitloop-superbuild NONE)

#set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/external_fetched" CACHE PATH "")
#set(FETCHCONTENT_BUILD_DIR "${FETCHCONTENT_BASE_DIR}/external_build" CACHE PATH "Shared build dir" FORCE)
#set(FETCHCONTENT_QUIET OFF)

include(FetchContent)  # must come immediately after, to lock in the value

add_subdirectory(framework)