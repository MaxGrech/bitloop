cmake_minimum_required(VERSION 3.21)
project(examples LANGUAGES CXX)

if (NOT TARGET bitloop::bitloop)
  find_package(bitloop CONFIG REQUIRED)
endif()

bitloop_add_dependency(test_sim)
bitloop_add_dependency(test_sim2)
bitloop_add_dependency(Mandelbrot)
bitloop_add_dependency(Airhockey)

# Add examples target executable
bitloop_new_project(examples)
