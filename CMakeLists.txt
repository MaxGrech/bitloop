# bitloop/CMakeLists.txt

cmake_minimum_required(VERSION 3.21)
project(bitloop-superbuild LANGUAGES CXX)

add_subdirectory(framework)

# Include examples in superbuild
bitloop_add_dependency(examples/test_sim)
#bitloop_add_dependency(examples/test_sim2)

# Add examples target executable
bitloop_new_project(examples)
