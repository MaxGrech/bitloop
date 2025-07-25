# bitloop/CMakeLists.txt

cmake_minimum_required(VERSION 3.21)
project(bitloop-superbuild LANGUAGES CXX)

add_subdirectory(framework)

# Add examples target executable
bitloop_add_simulation(launch_examples)

# Include examples in superbuild
bitloop_add_dependency(examples/test_sim)
bitloop_add_dependency(examples/test_sim2)